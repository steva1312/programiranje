const form = document.getElementById('form')
const setCharNum = document.getElementById('charNum')
const btn = document.getElementById('btn')
const pass = document.querySelector('.pass')
const uppercaseCheck = document.getElementById('uppercase')
const numCheck = document.getElementById('numbers')
const checkboxUppercase = document.querySelector('.checkbox-up')
const checkboxNumbers = document.querySelector('.checkbox-num')

checkboxUppercase.addEventListener('click', () => {
    checkboxUppercase.classList.toggle('checkbox-checked')
    uppercaseCheck.checked = !uppercaseCheck.checked
})

checkboxNumbers.addEventListener('click', () => {
    checkboxNumbers.classList.toggle('checkbox-checked')
    numCheck.checked = !numCheck.checked
})

form.addEventListener('submit', (e) => {
    e.preventDefault()
    const charNum = setCharNum.value
    const lowercase = fromLowToHigh(97, 122)
    const uppercase = fromLowToHigh(65, 90)
    const numbers = fromLowToHigh(48, 57)
    pass.style.color = 'black'
    let characters = lowercase

    if(uppercaseCheck.checked) {
        characters = characters.concat(uppercase)
    }
    if(numCheck.checked) {
        characters = characters.concat(numbers)
    }
    
    const passKey = []
    for(let i = 0; i < charNum; i++) {
        passKey.push(
            characters[Math.round(Math.random() * characters.length)]
        )
    }

    pass.textContent = passKey.join('')
})

function fromLowToHigh(low, high) {
    const array = []
    for(let i = low; i <= high; i++) {
        array.push(String.fromCharCode(i))
    }
    return array
}

checkboxUppercase.addEventListener('keypress', () => {
    checkboxUppercase.classList.toggle('checkbox-checked')
    uppercaseCheck.checked = !uppercaseCheck.checked
})

checkboxNumbers.addEventListener('keypress', () => {
    checkboxNumbers.classList.toggle('checkbox-checked')
    numCheck.checked = !numCheck.checked
})


