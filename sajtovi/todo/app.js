const input = document.querySelector('.mainInput')
const form = document.querySelector('.mainForm')
const todoContainer = document.querySelector('.todoContainer')
const body = document.querySelector('body')
const numCon = document.querySelector('.numCon')

let id = 0

const todoNum = document.createElement('h3')
numCon.appendChild(todoNum)
todoNum.innerText = `Number od todos: ${id}`
todoNum.classList.add('todoNum')
numCon.classList.add('numCon')

form.addEventListener('submit', e => {
    e.preventDefault()
    onSubmit(input.value)
    id++
    console.log(id)
    makeNum()
})

function onSubmit(name) {
    const newTodo = document.createElement('div')
    todoContainer.appendChild(newTodo)
    newTodo.innerText = name
    newTodo.classList.add('todo')

    const deleteTodo = document.createElement('span')
    newTodo.appendChild(deleteTodo)
    deleteTodo.innerText = '+'
    deleteTodo.classList.add('removeBtn')
    deleteTodo.addEventListener('click', () => {
        todoContainer.removeChild(newTodo)
    })
}

function makeNum() {
    const todoNum = document.createElement('h3')
    numCon.appendChild(todoNum)
    todoNum.innerText = `Number od todos: ${id}`
    todoNum.classList.add('todoNum')
    numCon.removeChild(numCon.firstChild)
}
