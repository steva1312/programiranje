const socket = io.connect('/room')

const roomname = ROOMNAME
const username = USERNAME
socket.emit('show-user', username)
socket.emit('join', roomname)

let opponentname
let player_index
let isPLayer
let xo
let xo_moves = 0
let turn_moves = 0
let tie_moves = 0
let gameover = false
let canceled = false
let moves = []

const combos = [
    [0, 1, 2],
    [3, 4, 5],
    [6, 7, 8],
    [0, 3, 6],
    [1, 4, 7],
    [2, 5, 8],
    [0, 4, 8],
    [2, 4, 6]
]

// DOM Elements
const p1_el = document.querySelector('.p1')
const p2_el = document.querySelector('.p2')
const s1_el = document.querySelector('.s1')
const s2_el = document.querySelector('.s2')
const turntex_el = document.querySelector('.next')

// Create table
const table_el = document.querySelector('.table')
for(let i = 0; i < 9; i++) {
    const tab = document.createElement('div')
    tab.classList.add('tab')
    tab.id = i
    table_el.appendChild(tab)
}

function join() {
    if (isPLayer) {
        playerFeatures()
    }
}

function playerFeatures() {
    const tabs = document.querySelectorAll('.tab')
    tabs.forEach(tab => {
        tab.addEventListener('click', () => {
            if(tab.textContent != '' || canceled)return
            if((xo === 'x' && xo_moves % 2 != 0) || (xo === 'o' && xo_moves % 2 != 1))return
            id = +tab.id
            moves.push(id) 
            socket.emit('set-table', id, xo)

            let point1 = 2
            let point2 = -2

            // Check for win
            combos.forEach(combo => {
                if(combo.every(e => moves.indexOf(e) != -1 )) {
                    gameover = true
                    socket.emit('game-result', `${username} won!`)
                    socket.emit('change-score', player_index)
                }
            })
            // Check for tie
            if(tie_moves === 8 && !gameover) {
                gameover = true
                socket.emit('game-result', 'Tie!')
                point1 = 1
                point2 = 1
            }
            if(gameover) {
                socket.emit('gameover', username, opponentname, point1, point2)
                socket.emit('show-btn')
                return
            }

            socket.emit('next-move')
        })
    })
    socket.on('show-btn', () => {
        const btn = document.createElement('button')
        btn.classList.add('game-btn')
        btn.textContent = 'Play Again'
        document.body.appendChild(btn)

        btn.addEventListener('click', () => {
            document.body.removeChild(btn)
            const header = document.createElement('header')
            header.textContent = 'Waiting for the opponent to accept ...'
            header.classList.add('game-btn')
            document.body.appendChild(header)
            socket.emit('add-play-again')
        })
    })
    socket.on('play-again', () => {
        document.body.removeChild(document.querySelector('.game-btn'))
        canceled = false
        gameover = false
        moves = []
        if(xo_moves % 2 === +player_index) socket.emit('set-turn-text', username)
    })
    socket.on('game-leave', () => {
        if(!canceled) socket.emit('gameover', username, opponentname, -2, 2, true)
    })
}

socket.on('is-player', bool => {
    isPLayer = bool
    join()
})
socket.on('get-table', table_array => {
    const tabs = document.querySelectorAll('.tab')
    for(let i = 0; i < table_array.length; i++) {
        tabs[i].textContent = table_array[i]
    }
})
socket.on('first-player', bool => {
    const waiting_el = document.querySelector('.waiting')
    if(bool) {
        waiting_el.style.display = 'block'
    } else {
        document.body.removeChild(waiting_el)
    }
})
socket.on('set-player-xo', str => { 
    xo = str 
    if(xo === 'o') {
        s1_el.id = '1'
        s2_el.id = '0'
        player_index = '1'
    } else {
        player_index = '0'
    }
})
socket.on('next-move', () => {
    xo_moves++
    tie_moves++
})
socket.on('send-opponent', () => socket.emit('set-opponent', username))
socket.on('set-opponent', name => { 
    opponentname = name
    p1_el.textContent = username
    p2_el.textContent = opponentname
})
socket.on('players-to-watchers', arr => {
    p1_el.textContent = arr[0]
    p2_el.textContent = arr[1]
})
socket.on('set-turn-text', () => socket.emit('set-turn-text', opponentname))
socket.on('change-turn-text', name => {
    if(name === username) {
        turntex_el.textContent = "It's your turn"
    } else {
        turntex_el.textContent = `Waiting for ${name}'s turn...`
    }
})
socket.on('change-score', idx => {
    const score_el = document.getElementById(idx)
    score_el.textContent = +score_el.textContent + 1
})
socket.on('game-result', str => turntex_el.textContent = str)
socket.on('gameover', () => {
    turn_moves++
    xo_moves = turn_moves
    tie_moves = 0
    canceled = true
    return
})
socket.on('clear-table', () => {
    const tabs = document.querySelectorAll('.tab')
    tabs.forEach(tab => {
        tab.textContent = ''
    })
})

// visitors
socket.on('v:change-turn-text', str => turntex_el.textContent = str)
socket.on('v:get-result', arr => {
    s1_el.textContent = arr[0]
    s2_el.textContent = arr[1]
})