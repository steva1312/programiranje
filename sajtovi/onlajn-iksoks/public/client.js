const socket = io()
const loginField = document.querySelector('.login')
const loginForm = document.querySelector('.login-form')
const loginInput = document.querySelector('.login-input')
const roomField = document.querySelector('.room')
const createForm = document.querySelector('.create-form')
const createInput = document.querySelector('.create-input')
const joinForm = document.querySelector('.join-form')
const joinInput = document.querySelector('.join-input')
const statusContainer = document.querySelector('.status-container')
const roomsEl = document.querySelector('.rooms')
const chatBox = document.querySelector('.chat-box')
const chatInput = document.querySelector('.chat-input')
const chatForm = document.querySelector('.chat-form')
sessionStorage.clear()
let username
let otherUsers

const clearEl = el => {
    while(el.firstChild) {
        el.removeChild(el.firstChild)
    }
}
const err = (str, input) => {
    input.setAttribute('placeholder', str)
    input.value = ''
    input.classList.add('err')
}
const messageEl = (ct, sender, you) => {
    const el = document.createElement('div')
    el.classList.add('msg-el')
    const senderEL = document.createElement('header')
    senderEL.textContent = sender
    senderEL.classList.add('sender')
    el.appendChild(senderEL)
    const textEl = document.createElement('header')
    textEl.textContent = `:  ${ct}`
    textEl.classList.add('msg-text')
    el.appendChild(textEl)
    chatBox.appendChild(el)
    if(you) {
        senderEL.style.fontStyle = 'italic'
    }
}

loginForm.addEventListener('submit', e => {
    e.preventDefault()
    if(loginInput.value === '' || loginInput.value[0] === ' ') {
        err('Invalid username', loginInput)
        return
    }
    if(otherUsers.indexOf(loginInput.value) != -1) {
        err('This name is already taken', loginInput)
        return
    }
    username = loginInput.value
    socket.emit('login', loginInput.value)
    loginInput.value = ''
    roomField.style.display = 'block'
    loginField.style.display = 'none'
    sessionStorage.setItem('username', username)
})

socket.on('set-users', users => {
    otherUsers = users.filter(user => user != username)
    clearEl(statusContainer)
    for(let i = 0; i < otherUsers.length; i++) {
        const activeUser = document.createElement('header')
        activeUser.textContent = otherUsers[i]
        statusContainer.appendChild(activeUser)
    }
})
socket.on('send-rooms', rooms => {
    clearEl(roomsEl)
    for(let i = 0; i < rooms.length; i++) {
        const RoomEl = document.createElement('header')
        RoomEl.classList.add('room-el')
        RoomEl.textContent = rooms[i]
        roomsEl.appendChild(RoomEl)
    }
})

joinForm.addEventListener('submit', e => {
    e.preventDefault()
    if(joinInput.value.indexOf(' ') != -1) {
        err('Can not contain spaces', joinInput)
        return
    }
    socket.emit('join', joinInput.value)
    window.open(document.location.href + joinInput.value, '_self')
})
chatForm.addEventListener('submit', e => {
    e.preventDefault()
    messageEl(chatInput.value, 'You', true)
    socket.emit('send-msg', {name : username, ct: chatInput.value})
    chatInput.value = ''
})

socket.on('send-msg', data => {
    messageEl(data.ct, data.name, false)
}) 