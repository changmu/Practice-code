'use strict';

let log = console.log;
let alert = log;

function showMenu({ title = "Menu", width = 100, height = 200 }={}) {
    alert( `${title} ${width} ${height}` );
}
  
showMenu({}); // Menu 100 200