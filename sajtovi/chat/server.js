const app = require('express')()
const http = require('http').createServer(app)
const io = require('socket.io')(http)
const activeUsers = []

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html')
})

io.on('connection', socket => {
    socket.on('login', name => {
        const userObj = {
            name: name,
            text: 'joined'
        }
        if(activeUsers.some(username => username === name)) {
            socket.emit('taken-username')
        } else {
            socket.broadcast.emit('user-text', userObj)
            activeUsers.push(name)
            socket.emit('handle-form')
            io.emit('get-usernames', activeUsers)
            socket.broadcast.emit('login', name)
        }
        socket.on('disconnect', () => {
            const userObj = {
                name: name,
                text: 'left'
            }
            activeUsers.splice(activeUsers.indexOf(name), 1)
            io.emit('get-usernames', activeUsers)
            socket.broadcast.emit('user-text', userObj)
        })
    })
    socket.on('send-message', data => {
        socket.broadcast.emit('send-message', data)
    })
})

const PORT = process.env.PORT || 3000
http.listen(PORT, () => console.log('server listening on port: ' + PORT))