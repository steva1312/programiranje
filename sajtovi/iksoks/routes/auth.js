const express = require('express')
const router = express.Router()
const User = require('./../model/User.js')
const passport = require('passport')

// Register
router.get('/register', isAlreadyAuthed, (req, res) => res.render('register'))
router.post('/register', (req, res) => {
    const { username, password, password2 } = req.body
    const errors = []

    // Checking for errors
    if(username.length < 3 || username.length > 15) errors.push('Unsupported username length')
    if(password != password2) errors.push("Passwords doesn't match")
    if(password.length < 6 || password.length > 20) errors.push('Unsupported password length')
    User.findOne({username: username})
        .then(user => { 
            if(user != null) errors.push('Account with that username already exist')

            // res.render() is in promise because err for acc that exist wouldn't show
            if(errors.length > 0) {
                res.render('register', {
                    errors,
                    username,
                    password,
                    password2
                })
            } else {
                // Save user
                const user = new User({
                    username: username,
                    password: password
                })
                user.save().then(() => {
                    console.log('New user registered!')
                    req.flash('succ_msg', 'Your registration is successful. You can login now.')
                    res.redirect('/account/login')
                }).catch(err => console.log(err))
            }
        })
        .catch(err => console.log(err))
})

// Login
router.get('/login', isAlreadyAuthed,(req, res) => res.render('login'))
router.post('/login', passport.authenticate('local', {
    successRedirect: '/',
    failureRedirect: '/account/login',
    failureFlash: true
}))

// Logout
router.delete('/logout', (req, res) => {
    req.logOut()
    res.redirect('/')
})

// Auth functions
function isAlreadyAuthed(req, res, next) {
    if(req.isAuthenticated()) return res.redirect('/')
    next()
}

module.exports = router