const express = require('express')
const app = express()
const server = require('http').createServer(app)
const path = require('path')
const io = require('socket.io')(server)
const users = []
const roomsName = []
const roomsforclient = []

app.use(express.static('public'))

app.get('/', (req, res) => {
    console.log(req.url)
    res.sendFile(path.join(__dirname, 'public', 'index.html'))
})

io.on('connection', socket => {
    socket.emit('set-users', users)
    socket.on('login', name => {
        users.push(name)
        socket.broadcast.emit('set-users', users)
        socket.on('send-msg', data => socket.broadcast.emit('send-msg', data))
        socket.emit('send-rooms', roomsforclient)
        socket.on('join', roomName => {
            if(roomsName.indexOf(roomName) > -1) {
                // Join room
            } else {
                // Create room
                roomsName.push(roomName)
                const roomData = {
                    players: 0,
                    canceled: false
                }
                app.get(`/${roomName}`, (req, res) => {
                    res.sendFile(path.join(__dirname, 'public', 'room.html'))
                })
                const room = io.of(`/${roomName}`)
                room.on('connection', socket => {
                    if(roomsforclient.indexOf(roomName) === -1) {
                        roomsforclient.push(roomName)
                        io.emit('send-rooms', roomsforclient)
                    }
                    if(roomData.players === 2 || roomData.canceled === true) {
                        socket.emit('full')
                        return
                    } else if (roomData.players === 0) {
                        socket.emit('xoro', 'x')
                    } else if (roomData.players === 1) {
                        socket.emit('xoro', 'o')
                        room.emit('started')
                    } 
                    socket.on('send-opponent', name => {
                        socket.broadcast.emit('set-opponent', name)
                    })
                    socket.on('send-id-to-server', data => {
                        socket.broadcast.emit('get-tab-id', data)
                    })
                    socket.on('handle-move', () => {
                        socket.broadcast.emit('handle-move')
                    })
                    socket.on('win', () => {
                        socket.broadcast.emit('win')
                    })
                    socket.on('tie', () => {
                        socket.broadcast.emit('tie')
                    })
                    socket.on('send-message', msg => {
                        socket.broadcast.emit('receive-message', msg)
                    })
                    roomData.players++
                    socket.on('disconnect', () => {
                        if(roomData.players === 1) {
                            roomData.canceled = false
                            roomsforclient.splice(roomsforclient.indexOf(roomName), 1)
                            io.emit('send-rooms', roomsforclient)
                            socket.broadcast.emit('alone', 'Reload to join the room.')
                        } else if (roomData.players === 2) {
                            roomData.canceled = true
                            socket.broadcast.emit('alone', 'Your opponent left the room. Reload website so he can join again.')
                        }
                        roomData.players--
                    })
                })
            }
        })
        socket.on('disconnect', () => {
            users.splice(users.indexOf(name), 1)
            socket.broadcast.emit('set-users', users)
        })
    })
})

const PORT = process.env.PORT || 3000
server.listen(PORT, console.log('listening on port: ' + PORT))