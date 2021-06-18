const socket = io.connect(location.href)
const body = document.querySelector('body')
const gameField = document.querySelector('.game')
const fullHeader = document.querySelector('.full')
let username = sessionStorage.getItem('username')
const topic = document.querySelector('.topic')
const statusEl = document.querySelector('.status')
const usernameEl = document.querySelector('.user-name')
const opponentnameEl = document.querySelector('.opponent-name')
const userScore = document.querySelector('.user-score')
const opponentScore = document.querySelector('.opponent-score')
const table = document.querySelector('.table')
const xoroTurnEl = document.querySelector('.xoro-turn')
const gameoverEl = document.querySelector('.game-over')
const typeGameover = document.querySelector('.end-type')
const counter = document.querySelector('.counter')
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
let playerMoves = []
let xoro
let opponent
let xoroTurn = 0
let tieMoves = 0
let turnNum
let win = false
let canceled = false
if(username === null) {
    username = 'bezimeni'
}

const clearEl = el => {
    while(el.firstChild) {
        el.removeChild(el.firstChild)
    }
}
const gameOver = winText => {
    xoroTurnEl.textContent = ''
    gameoverEl.style.visibility = 'visible'
    typeGameover.textContent = winText
    tieMoves = 0
    playerMoves = []
    win = false
    canceled = true
    let timeLeft = 10
    let t
    const countDown = () => {
        counter.textContent = timeLeft
        timeLeft--
        if(timeLeft === -1) {
            clearTimeout(t)
            gameoverEl.style.visibility = 'hidden'
            canceled = false
            startGame()
        }
    }
    t = setInterval(countDown, 1000)
}

const startGame = () => {
    if((xoroTurn % 2 - turnNum) === 0) {
        xoroTurnEl.textContent = "It's your move ..."
    } else {
        xoroTurnEl.textContent = `Waiting for ${opponent}'s move ...`
    }    
    clearEl(table)
    for(let i = 0; i < 9; i++) {
        const tab = document.createElement('div')
        tab.classList.add('tab')
        tab.id = i
        table.appendChild(tab)
    }
    const tabs = document.querySelectorAll('.tab')
    tabs.forEach(tab => {
        tab.addEventListener('click', () => {
            const handleClick = () => {
                if(canceled)return
                if(tab.textContent.length === 0) {
                    tab.textContent = xoro
                    playerMoves.push(tab.id)
                    socket.emit('send-id-to-server', {id : tab.id, xoro: xoro})
                    xoroTurn++
                    tieMoves++
                    socket.emit('handle-move')
                    xoroTurnEl.textContent = `Waiting for ${opponent}'s move ...`
                    combos.forEach(combo => {
                        if(combo.every(e => playerMoves.indexOf(e + '') > -1)) {
                            userScore.textContent = +userScore.textContent + 1
                            socket.emit('win')
                            win = true
                            gameOver('You won!')
                        }
                    })
                    if(tieMoves === 9 && win === false) {
                        gameOver('Tie!')
                        socket.emit('tie')
                    }
                }
            }
            if((xoro === 'x' && xoroTurn % 2 === 0) || (xoro === 'o' && xoroTurn % 2 === 1)) {
                handleClick()
            } 
        })
    })
}

socket.on('full', () => {
    body.removeChild(gameField)
    fullHeader.style.display = 'block'
})
socket.on('xoro', _xoro => {
    xoro = _xoro
    if(sessionStorage.getItem('xoro')) {
        xoro = sessionStorage.getItem('xoro')
    } else {
        sessionStorage.setItem('xoro', _xoro)
    }
    if(xoro === 'x') {
        turnNum = 0
    } else {
        turnNum = 1
    }
})
socket.on('started', () => {
    socket.emit('send-opponent', username)
})
socket.on('set-opponent', name => {
    opponent = name
    document.querySelector('.waiting').style.display = 'none'
    topic.style.visibility = 'visible'
    usernameEl.textContent = username
    opponentnameEl.textContent = opponent
    startGame()
})
socket.on('get-tab-id', data => {
    document.getElementById(data.id).textContent = data.xoro
})
socket.on('handle-move', () => {
    xoroTurn++
    tieMoves++
    xoroTurnEl.textContent = "It's your move ..."
})
socket.on('win', () => {
    opponentScore.textContent = +opponentScore.textContent + 1
    win = true
    tieMoves = 0
    gameOver(opponent + ' won!')
})
socket.on('tie', () => {
    gameOver('Tie!')
})
socket.on('alone', ct => {
    document.querySelector('.alone').style.display = 'block'
    document.querySelector('.alone').textContent = ct
    gameField.style.display = 'none'
    document.querySelector('.full').style.display = 'none'
})

// Chat
const chatForm = document.querySelector('.chat-form')
const chatInput = document.querySelector('.chat-input')
const chatBox = document.querySelector('.chatbox')

const createMsg = (msg, classList) => {
    if(msg === '')return
    const msgEl = document.createElement('header')
    msgEl.classList.add('msg', classList)
    msgEl.textContent = msg
    chatBox.appendChild(msgEl)
}

chatForm.addEventListener('submit', e => {
    e.preventDefault()
    createMsg(chatInput.value, 'typer')
    socket.emit('send-message', chatInput.value)
    chatInput.value = ''
})

socket.on('receive-message', msg => createMsg(msg, 'receiver'))