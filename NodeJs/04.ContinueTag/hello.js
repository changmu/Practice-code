import {user} from './user.js';

console.log(user); // no such variable (each module has independent variables)
document.body.innerHTML = user; // John

// alert(import.meta.url);