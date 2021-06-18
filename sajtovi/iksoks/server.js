const express = require('express')
const app = express()
const server = require('http').createServer(app)
const mongoose = require('mongoose')
const passport = require('passport')
const flash = require('express-flash')
const session = require('express-session')
const methodOverride = require('method-override')
const User = require('./model/User.js')
require('dotenv/config')

// Passport config 
require('./passport.js')(passport)

// Middlewares und requirements for auth
app.set('view engine', 'ejs')
app.use(express.static('./public'))
app.use(methodOverride('_method'))
app.use(express.urlencoded({ extended: true}))
app.use(session({
    secret: 'secret987',
    resave: true,
    saveUninitialized: true
}))
app.use(passport.initialize())
app.use(passport.session())
app.use(flash())
app.use((req, res, next) => {
    res.locals.succ_msg = req.flash('succ_msg')
    next()
})
app.use((req, res, next) => {
    if(req.url === '/account/login' || req.url === '/account/register') return next()
    if(req.isAuthenticated()) return next()
    res.redirect('/account/login')
})

// Routes
app.get('/', (req, res) => {
    res.render('index', { username: req.user.username})
})
app.use('/account', require('./routes/auth.js'))
app.get('/leaderboard', async (req, res) => {
    const users = await User.find()
    users.sort((a, b) => {
        if(b.score - a.score === 0) {
            return a.date - b.date
        } else {
            return b.score - a.score
        }
    })

    res.render('leaderboard', {
        users: users,
        username: req.user.username
    })
})

// Connect to db
mongoose.connect(process.env.DB, { useNewUrlParser: true, useUnifiedTopology: true },err => {
    if(err) {
        throw err
    } else {
        console.log('Connected to DB!')
    }
})

// Socket io
const io = require('socket.io')(server)
const active_users = []
const active_rooms = []

const home = io.of('/home')
home.on('connection', socket => {
    let user_data
    socket.on('show-user', username => {
        user_data = { username: username, home: true}
        
        if(active_users.find(user => user.username === user_data.username)) {
            socket.emit('already')
            return
        } else {
            active_users.push(user_data)
            home.emit('send-users', active_users)
            socket.emit('send-rooms', active_rooms)
        }

        socket.on('disconnect', () => {
            active_users.splice(active_users.indexOf(user_data), 1)
            home.emit('send-users', active_users)
        })
    })
})

const room = io.of('/room')
room.on('connection', socket => {
    let user_data
    socket.on('show-user', username => {
        user_data = { username: username, home: false}
        active_users.push(user_data)
        home.emit('send-users', active_users)
    })

    let Room
    socket.on('join', roomname => {
        socket.join(roomname)

        const search_room = active_rooms.find(_room => _room.name === roomname )
        if(search_room) {
            // Join the Room
            Room = search_room
        } else {
            // 'Create' the Room
            Room = { name: roomname, users: [], players: [], table: Array(9).fill(''), turnText: '', result: [0, 0], again: 0, canceled: false }
            active_rooms.push(Room)
        }

        Room.users.push(user_data.username)
        if(Room.users.length <= 2) {
            Room.players.push(user_data.username)
            user_data.isPlayer = true
            socket.emit('is-player', true)
        } else socket.emit('is-player', false)

        if(Room.users.length === 1) {
            socket.emit('first-player', true)
            socket.emit('set-player-xo', 'x')
        } else if (Room.users.length === 2) {
            socket.broadcast.to(roomname).emit('first-player', false)
            socket.emit('set-player-xo', 'o')
            room.to(roomname).emit('send-opponent')
            room.to(roomname).emit('change-turn-text', Room.players[0])
        } else if (Room.users.length > 2) {
            socket.emit('players-to-watchers', Room.players)
            socket.emit('v:change-turn-text', Room.turnText)
            socket.emit('v:get-result', Room.result)
        }

        socket.emit('get-table', Room.table)
        home.emit('send-rooms', active_rooms)

        socket.on('set-table', (id, xo) => {
            Room.table[id] = xo
            room.to(roomname).emit('get-table', Room.table)
        })
        socket.on('next-move', () => {
            room.to(roomname).emit('next-move')
            socket.emit('set-turn-text')
        })
        socket.on('set-opponent', name => socket.broadcast.to(roomname).emit('set-opponent', name))
        socket.on('set-turn-text', name => {
            Room.turnText = `Waiting for ${name}'s move ...`
            room.to(roomname).emit('change-turn-text', name)
        })
        socket.on('change-score', idx => {
            room.to(roomname).emit('change-score', idx)
            Room.result[+idx] += 1
        })
        socket.on('game-result', str => {
            room.to(roomname).emit('game-result', str)
            Room.turnText = str
        })
        socket.on('gameover', (n1, n2, p1, p2, left) => {
            room.to(roomname).emit('gameover')

            updateScore(n1, p1)
            updateScore(n2, p2)

            async function updateScore(name, points) {
                const player = await User.findOne({username: name})
                player.score += points
                await player.save()
            }

            if(left) Room.canceled = true
            console.log(Room)
        })
        socket.on('show-btn', () => {
            room.to(roomname).emit('show-btn')
        })
        socket.on('add-play-again', () => {
            Room.again += 1
            if(Room.again === 2) {
                Room.table = Array(9).fill('')
                room.to(roomname).emit('play-again')
                room.to(roomname).emit('clear-table')
                Room.again = 0
            }
        })
    })

    socket.on('disconnect', () => {
        if(Room === undefined)return
        active_users.splice(active_users.indexOf(user_data), 1)
        home.emit('send-users', active_users)

        Room.users.splice(Room.users.indexOf(user_data.username), 1)
        if(Room.users.length === 0) {
            active_rooms.splice(active_rooms.indexOf(room))
        }

        home.emit('send-rooms', active_rooms)

        socket.emit('game-leave')
    })
})

app.post('/room', (req, res) => {
    res.render('room', {room: req.body.join, username: req.user.username})
})

// Set up the server
const PORT = process.env.PORT || 4000
server.listen(PORT, () => console.log(`Listening on port: ${PORT}`))