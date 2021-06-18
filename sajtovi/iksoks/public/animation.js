const container = document.createElement('div')
container.classList.add('xo-container')
container.setAttribute('animation', '')
document.body.appendChild(container)
const width = window.innerWidth
const height = window.innerHeight

setInterval(summonImg, 250);

function summonImg() {
    const xoNum = Math.random() * 10
    const xo = xoNum > 5 ? 'o' : 'x'
    const xoEl = document.createElement('img')
    xoEl.setAttribute('src', `/img/${xo}.png`)
    xoEl.classList.add('xo-img')
    xoEl.style.left = Math.random() * width + 'px'
    xoEl.animate([{
        left: xoEl.style.left,
        top: '0px'
    }, {
        left: Math.random() * width + 'px',
        top: height - 30 + 'px'
    }], 5000)
    container.appendChild(xoEl)
    setTimeout(() => {
        container.removeChild(xoEl)
    }, 5000);
}