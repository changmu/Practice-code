'use strict';

let log = console.log;
let alert = log;


function User(name, birthday) {
    this.name = name;
    this.birthday = birthday;
  
    // 年龄是根据当前日期和生日计算得出的
    Object.defineProperty(this, "age", {
      get() {
        let todayYear = new Date().getFullYear();
        return todayYear - this.birthday.getFullYear();
      }
    });
  }
  
  let john = new User("John", new Date(1992, 6, 1));
  
  alert( john.birthday ); // birthday 是可访问的
  alert( john.age );      // ……age 也是可访问的
  log(Object.getOwnPropertyDescriptor(john, "age"))