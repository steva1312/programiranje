const socket = io.connect('/home')

const username = USERNAME
socket.emit('show-user', username)

const body = document.querySelector('body')

socket.on('send-users', users => {
    const container = document.querySelector('.users-con')
    clearElement(container)

    if(users.length > 1) {
        users.forEach(user => {
            if(user.username != username) {
                const user_el = document.createElement('header')
                user_el.textContent = user.username
                user_el.classList.add('topic-el', user.home ? 'green' : 'yellow')
                container.appendChild(user_el)
            }
        })

        const icon = document.querySelector('.fa-users')
        numCricle(users.length - 1, icon)
    } else {
        const alone = document.createElement('header')
        alone.classList.add('alone')
        alone.textContent = "There isn't any active user."
        container.appendChild(alone)
    }
})

socket.on('send-rooms', rooms => {
    const container = document.querySelector('.rooms-con')
    clearElement(container)

    if(rooms.length > 0) {
        rooms.forEach(room => {
            const room_el = document.createElement('div')
            const room_name = document.createElement('header')
            room_name.textContent = room.name
            room_name.classList.add('topic-el', 'green')
            const ul = document.createElement('ul')
            room.users.forEach(user => {
                const user_el = document.createElement('li')
                user_el.textContent = user
                user_el.classList.add('topic-el', 'yellow')
                ul.appendChild(user_el)
            })
            room_el.appendChild(room_name)
            room_el.appendChild(ul)
            container.appendChild(room_el)
        })

        const icon = document.querySelector('.fa-boxing-glove')
        numCricle(rooms.length, icon)
    } else {
        const alone = document.createElement('header')
        alone.classList.add('alone')
        alone.textContent = "There isn't any available room."
        container.appendChild(alone)
    }
})

// If there is already online user with that account
socket.on('already', () => {
    clearElement(body)
    const header = document.createElement('header')
    header.textContent = 'Connection Error'
    header.classList.add('main-header')
    const p = document.createElement('p')
    p.classList.add('p-center')
    p.textContent = 'There is already online user connected to this account. Log out and connect to another one or wait for him to leave. Note that if you are logged in on another tab right now, you will log out from there if you click the button below.'
    const form = document.createElement('form')
    form.classList.add('form-center')
    form.setAttribute('method', 'POST')
    form.setAttribute('action', '/account/logout?_method=DELETE')
    const btn = document.createElement('button')
    btn.textContent = 'Log Out'
    btn.setAttribute('type', 'submitt')
    form.appendChild(btn)
    body.appendChild(header)
    body.appendChild(p)
    body.appendChild(form)
})

function clearElement(el) {
    while(el.firstChild) {
        el.removeChild(el.firstChild)
    }
}

function numCricle(num, container) {
    const el = document.createElement('div')
    el.classList.add('circle')
    el.textContent = num
    container.appendChild(el)
}