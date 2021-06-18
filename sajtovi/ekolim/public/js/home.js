const slogani = ['-"Kada sneg veje, limenka te greje"', '-"Limenku daj i Zemlji vrati sjaj"', '-"Našu kompaniju podrži da ti život bude duži"'];
const sloganEl = document.querySelector('.najslogan');
const vreme = 7000;
const vremeAn = 500;
let idx = 0;

function zameni() {
    sloganEl.textContent = slogani[idx];
    sloganEl.style.animation = `slogan-u ${vremeAn}ms ease-in`;

    setTimeout(() => {
        sloganEl.style.animation = `slogan-van ${vremeAn}ms ease-out`;

        setTimeout(() => {
            idx++;
            if (idx == slogani.length) idx = 0;

            zameni();
        }, vremeAn);
    }, vreme);


}

zameni();


const skrolBtn = document.querySelector('.skrol-btn');
const header = document.querySelector('header');
skrolBtn.addEventListener('click', () => {
    window.scrollBy(0, window.innerHeight - header.getBoundingClientRect().height)
})