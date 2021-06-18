const burger = document.querySelector('.burger');
const nav = document.querySelector('nav');
const navLinks = document.querySelectorAll('nav ul li');

burger.addEventListener('click', () => {

    nav.classList.toggle('nav-show');

    navLinks.forEach((link, index) => {
        if (link.style.animation) {
            link.style.animation = '';
        } else {
            link.style.animation = link.style.animation = `navLinkFade 500ms ease-in forwards ${index / 7 * 1.2}s`;
        }
    })

    burger.classList.toggle('toggle');
});