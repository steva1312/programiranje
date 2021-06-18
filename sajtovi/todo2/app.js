const listContainer = document.querySelector('.list-container')
const listForm = document.querySelector('.list-form')
const listInput = document.querySelector('.input-list')
const todoField = document.querySelector('.todo-field')
const todoFieldListName = document.querySelector('.list-name')
const todoContainer = document.querySelector('.todo-container')
const todoForm = document.querySelector('.todo-form')
const todoInput = document.querySelector('.input-todo')
const removeClearedTodosBtn = document.querySelector('.rmvClrdTodosBtn')
todoField.style.display = 'none'
let listIdArr = []

const clearEl = (el) => {
    while(el.firstChild) {
        el.removeChild(el.firstChild)
    }
}

const setLSItem = (key, value) => {
    localStorage.setItem(`${key}`, JSON.stringify(value))
}

const listObjFactory = (name, id) => {
    return {
        name: name,
        id: id,
        todos: []
    }
}

const todoObjFactory = (name, id) => {
    return {
        name: name,
        id: id, 
        cleared: false
    }
}

const getAndUpdateLSTodos = (listId, isFromForm) => {
    const LSList = JSON.parse(localStorage.getItem(listId))
    const LStodos = LSList.todos
    
    if (isFromForm) {
        const todoId = Date.now()
        const todoInfo = todoObjFactory(todoInput.value, todoId)
        LStodos.push(todoInfo)
        LSList.todos = LStodos
        localStorage.setItem(listId, JSON.stringify(LSList))
        makeTodo(todoId, todoInput.value, listId)
    } else {
        for(let i = 0; i < LStodos.length; i++){
            makeTodo(LStodos[i].id, LStodos[i].name, listId, LStodos[i].cleared)
        }
    }
}

const makeTodo = (todoId, todoTextContent, listId, isCleared) => {
    const todoBox = document.createElement('div')
    todoBox.classList.add('todo-box')
    todoContainer.appendChild(todoBox)
    todoBox.id = `todo-box${todoId}`

    const todoInput = document.createElement('input')
    todoInput.setAttribute('type', 'checkbox')
    todoBox.appendChild(todoInput)
    todoInput.id = todoId

    const todoElement = document.createElement('label')
    todoElement.classList.add('todo-name')
    todoElement.setAttribute('for', todoId)
    todoElement.textContent = todoTextContent
    todoBox.appendChild(todoElement)

    if (isCleared) {
        todoInput.checked = true
    }

    todoInput.addEventListener('change', () => {
        const LSList = JSON.parse(localStorage.getItem(listId))
        const LStodos = LSList.todos
        const selectedTodo = LStodos.filter(todo => {
            return todo.id === todoId
        })[0]
        LStodos[LStodos.indexOf(selectedTodo)].cleared = !LStodos[LStodos.indexOf(selectedTodo)].cleared
        LSList.todos = LStodos
        localStorage.setItem(listId, JSON.stringify(LSList))
    })
}

const handleTodoField = (headerText, id) => {
    todoFieldListName.textContent = headerText
    todoField.id = id
    todoField.style.display = ''
}

const createListElement = (textContent, id, isFromForm) => {
    const listBox = document.createElement('div')
    listBox.classList.add('list-box')
    listContainer.appendChild(listBox)

    const listElement = document.createElement('span')
    listElement.textContent = textContent
    listElement.classList.add('list')
    listBox.appendChild(listElement)

    const removeListBtn = document.createElement('i')
    removeListBtn.classList.add('far', 'fa-minus-circle')
    listBox.appendChild(removeListBtn)
    removeListBtn.addEventListener('click', () => {
        if (id + '' === todoField.id) {
            todoField.style.display = 'none'
        }
        listIdArr = listIdArr.filter(_id => _id != id)
        setLSItem('listIdArr', listIdArr)
        localStorage.removeItem(`${id}`)
        listContainer.removeChild(listBox)
    })

    const addListStyle = () => {
        const listElements = document.querySelectorAll('.list')
        for(let i = 0; i < listElements.length; i++ ) {
            listElements[i].style.fontWeight = 'unset'
        }
        listElement.style.fontWeight = 'bolder'
    }
    if (isFromForm) {
        addListStyle()
    }
    listElement.addEventListener('click', () => {
        clearEl(todoContainer)
        handleTodoField(textContent, id)
        addListStyle()
        getAndUpdateLSTodos(id, null, false)
    })
}

const handlePageLoad = () => {
    if (localStorage.length > 1) {
        listIdArr = JSON.parse(localStorage.getItem('listIdArr'))
        for(let i = 0; i < listIdArr.length; i++) {
            createListElement(JSON.parse(localStorage.getItem(listIdArr[i])).name, listIdArr[i])
        }
    } else {
        setLSItem('listIdArr', [])
    }
}

const handleListFormSubmit = e => {
    e.preventDefault()
    if(listInput.value === '' || listInput.value[0] === ' ')return
    const id = Date.now()
    const listInfo = listObjFactory(listInput.value, id)
    createListElement(listInput.value, id, true)
    setLSItem(id, listInfo)
    listIdArr.push(id)
    setLSItem('listIdArr', listIdArr)
    handleTodoField(listInput.value, id)
    clearEl(todoContainer)

    listInput.value = null
}

const handleTodoFormSubmit = (e) => {
    e.preventDefault()
    if(todoInput.value === '' || todoInput.value[0] === ' ')return
    getAndUpdateLSTodos(todoField.id, true)

    todoInput.value = null
}

const handleRemoveTodosBtn = () => {
    const LSList = JSON.parse(localStorage.getItem(todoField.id))
    const LStodos = LSList.todos
    const unclearedTodos = LStodos.filter(todo => {
        return todo.cleared === false
    })
    const clearedTodos = LStodos.filter(todo => {
        return todo.cleared === true
    })
    for(let i = 0; i < clearedTodos.length; i++) {
        todoContainer.removeChild(document.getElementById(`todo-box${clearedTodos[i].id}`))
    }
    LSList.todos = unclearedTodos
    localStorage.setItem(todoField.id, JSON.stringify(LSList))
}

window.addEventListener('load', handlePageLoad, false)
listForm.addEventListener('submit', handleListFormSubmit, false)
todoForm.addEventListener('submit', handleTodoFormSubmit, false)
removeClearedTodosBtn.addEventListener('click', handleRemoveTodosBtn, false)

document.querySelector('.resetStorage').addEventListener('click', () => localStorage.clear())