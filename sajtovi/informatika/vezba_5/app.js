const header = document.querySelector('.animate')
const text = header.textContent

header.textContent = ''
let currentWidth = 0
const letterSpacing = 7
for(let i = 0; i < text.length; i++) {
    const span = document.createElement('span')

    span.style.fontFamily = 'fantasy'
    span.style.left = currentWidth + 'px'
    span.style.animationDelay = i * 100 + 'ms'
    span.classList.add('animate-letter')

    span.textContent = text[i]
    header.appendChild(span)
    currentWidth += span.clientWidth + 10
}