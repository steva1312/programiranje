window.addEventListener('load', () => {
    const pads = document.querySelectorAll('.pad-container div')
    const sounds = document.querySelectorAll('.sound')
    const colors = ["rgb(3, 226, 33)", "rgb(255, 0, 0)", "rgb(55, 199, 209)", "rgb(211, 226, 3)", "rgb(226, 3, 170)", "rgb(3, 7, 226)"]
    const padCons = document.querySelectorAll('.pad-container')

    pads.forEach( (pad, index) => {
        pad.addEventListener('click', () => {
            sounds[index].currentTime = 0
            sounds[index].play()
            createBall(index)
        
        } )
    })

    window.addEventListener('keyup', (e) => {
        if(e.keyCode === 65) {
            sounds[0].currentTime = 0
            sounds[0].play()
            createBall(0)
        }
        if(e.keyCode === 83) {
            sounds[1].currentTime = 0
            sounds[1].play()
            createBall(1)
        }
        if(e.keyCode === 68) {
            sounds[2].currentTime = 0
            sounds[2].play()
            createBall(2)
        }
        if(e.keyCode === 74) {
            sounds[3].currentTime = 0
            sounds[3].play()
            createBall(3)
        }
        if(e.keyCode === 75) {
            sounds[4].currentTime = 0
            sounds[4].play()
            createBall(4)
        }
        if(e.keyCode === 76) {
            sounds[5].currentTime = 0
            sounds[5].play()
            createBall(5)
        }
    })


    function createBall(index) {
        const ball = document.createElement('div')
            padCons[index].appendChild(ball)
            ball.classList.add('ball')
            ball.style.background = colors[index]
            ball.style.animation = 'ballAnime 6s linear'

            ball.addEventListener('animationend', () => {
                padCons[index].removeChild(ball)
            })
    }
})