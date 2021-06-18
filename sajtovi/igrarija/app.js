const cells = document.querySelectorAll('.cell')
const endHeader = document.querySelector('.gameEndHeader')
const endDiv = document.querySelector('.endDiv')
const btn = document.querySelector('.restartBtn')

const xAudio = document.querySelectorAll('.x-audio')
const oAudio = document.querySelectorAll('.o-audio')
const tAudio = document.querySelectorAll('.t-audio')

const combos = [
    [0,1,2],
    [3,4,5],
    [6,7,8],
    [0,3,6],
    [1,4,7],
    [2,5,8],
    [0,4,8],
    [2,4,6]
]

let movesX = []
let movesO = []

let xoroWon = false
let gameEnd = false
let moves = 0
let xoroMoves = 0
let tieMoves = 0

function xoroStyle() {
    cells.forEach(cell => {
        if(moves % 2 === 0) {
                cell.classList.remove('o')
                cell.classList.add('x')
        } else {
            cell.classList.remove('x')
            cell.classList.add('o')
        }
    })
}
xoroStyle()

function handleClick(e) {
    const id = +e.target.id
    xoro(id)
    tieCheck()
    xoroStyle()
    cells[id].classList.remove('x', 'o')
    if(gameEnd) {
        endDiv.style.display = 'flex'
    }
}

function gameEndText(text) {
    endHeader.textContent = text
}

function tieCheck() {
    if(gameEnd === false && tieMoves === 9) {
        gameEnd = true
        gameEndText('Izjednačenje!')
        tAudio[Math.floor(Math.random() * 3)].play()
    }
}

function xoro(id) {
    if(moves % 2 === 0) {
        setTextContent('x', id)
        movesX.push(id)
        declareWinner(movesX, 'X je pobednik!')
        cells[id].classList.add('red')
        if(xoroWon) {
            xAudio[Math.floor(Math.random() * 3)].play()
        }
    } else {
        setTextContent('o', id)
        movesO.push(id)
        declareWinner(movesO, 'O je pobednik!')
        cells[id].classList.add('yellow')
        if(xoroWon) {
            oAudio[Math.floor(Math.random() * 3)].play()
        }
    }
    moves++
    tieMoves++
}

function declareWinner(xoro, text) {
    const winComboArr = []
    combos.forEach(combo => {
        if(combo.every(n => {return xoro.indexOf(n) != -1})) {
            winComboArr.push(true)
        } else {
            winComboArr.push(false)
        }
    })
    const winCombo = combos[winComboArr.indexOf(true)]
    if(winCombo !== undefined) {
        winCombo.forEach(n => {
            cells[n].style.color = 'white'
        })
        gameEndText(text)
        gameEnd = true
        xoroWon = true
    }
}

function setTextContent(l, id) {
    cells[id].textContent = `${l}`
}

cells.forEach(cell => {
    cell.addEventListener('click', (e) => {
        if(cell.textContent || gameEnd) {
            return null
        } else {
            return handleClick(e)
        }
    })
})

btn.addEventListener('click', () => {
    cells.forEach(cell => {
        cell.textContent = null
        cell.style.color = 'black'
        cell.classList.remove('red', 'yellow')
    })
    xoroMoves++
    tieMoves = 0
    gameEnd = false
    xoroWon = false
    movesX = []
    movesO = []
    moves = xoroMoves
    xoroStyle()
    endDiv.style.display = 'none'
})