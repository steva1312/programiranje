const express = require('express');
const nodemailer = require('nodemailer');
require('dotenv').config();

const app = express();

app.set('view engine', 'ejs');
app.use(express.static('public'));
app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
    res.render('index');
})

app.get('/kontakt', (req, res) => {
    res.render('contact');
})

app.get('/o-nama', (req, res) => {
    res.render('about');
})

app.get('/naruci', (req, res) => {
    res.render('order');
})

app.get('/gostovanja', (req, res) => {
    res.render('gostovanja');
})

app.post('/send', async (req, res) => {
    const transporter = nodemailer.createTransport({
        service: 'Gmail',
        auth: {
            user: 'ekolim.nis@gmail.com',
            pass: 'anaivicgas',
        }
    });

    await transporter.sendMail({
        to: 'ekolim.nis@gmail.com',
        subject: 'Poruka klijenta',
        html: `
        <h1>${req.body.name}</h1>
        <p>${req.body.msg}</p>
        <p>${req.body.mail}</p>
        `
    })

    res.send('Poruka je uspešno poslata, možete se vratiti na početnu stranicu putem ovog <a href="/">linka</a>');
})

app.post('/naruci', async (req, res) => {
    const transporter = nodemailer.createTransport({
        service: 'Gmail',
        auth: {
            user: 'ekolim.nis@gmail.com',
            pass: 'anaivicgas',
        }
    });
    await transporter.sendMail({
        to: 'ekolim.nis@gmail.com',
        subject: 'Narudžbina klijenta',
        html: `
        <h1>Ime i prezime: ${req.body.name}</h1>
        <h3>Mejl adresa: ${req.body.mail}</h3>
        <h3>Broj telefona: ${req.body.phone}</h3>
        <h3>Adresa stanovanja: ${req.body.adress}</h3>
        <h3>Poštanski broj: ${req.body.zip}</h3>
        <p>Napomena: ${req.body.tip}</p>
        `
    })

    res.send('Uspešno ste naručili proizvod, stupićemo u kontakt sa Vama u roku od 12 sati, možete se vratiti na početnu stranicu putem ovog <a href="/">linka</a>')
})

const PORT = process.env.PORT || 3000;
app.listen(PORT, console.log('Server is running...'));