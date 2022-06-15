'use strict';

let log = console.log;
let alert = log;


let animal = {
    eats: true,
    walk() {
      alert("Animal walk");
    }
  };
  
  let rabbit = {
    jumps: true,
    __proto__: animal
  };
  
  // walk 方法是从原型中获得的
  rabbit.walk(); // Animal walk