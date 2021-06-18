const pads = document.querySelectorAll('.pad')
const audio = document.querySelectorAll('audio')

pads.forEach(pad => {
  pad.addEventListener('click', () => {
    const currentAudio = document.getElementById(pad.getAttribute('note'))
    currentAudio.currentTime = 0
    currentAudio.play()
    startStyle(pad, currentAudio)
  })

  window.addEventListener('keydown', (e) => {
    const currentAudio = document.getElementById(pad.getAttribute('note'))
    if(e.repeat) return
    if(e.key === pad.getAttribute('key')) {
      currentAudio.play()
      currentAudio.currentTime = 0
      startStyle(pad, currentAudio)
    }
    console.log(e)
  })
})

function startStyle (pad, currentAudio) {
  pad.classList.add('clicked')
  currentAudio.addEventListener('ended', () => {
    pad.classList.remove('clicked')
  })
}

