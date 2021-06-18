const LocalStrategy = require('passport-local')
const User = require('./model/User.js')

module.exports = function(passport) {
    passport.use(
        new LocalStrategy((username, password, done) => {
            User.findOne({ username : username}).then(user => {
                if(!user) return done(null, false, { message: "User with that username doesn't exist"})
                if(user.password != password) return done(null, false, { message: "That password is incorrect"})
                return done(null, user)
            }).catch(err => {
                return done(err)
            })
        })
    )

    passport.serializeUser((user, done) => {
        done(null, user.id);
      });
      
    passport.deserializeUser((id, done) => {
        User.findById(id, (err, user) => {
            done(err, user)
        })
    })
}