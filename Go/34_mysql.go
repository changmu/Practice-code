package main

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

func main() {
	db, err := sql.Open("mysql", "qiu:@tcp(192.168.126.10:3306)/?charset=utf8")
	checkError(err)

	// insert
	stmt, err := db.Prepare("INSERT INTO test.userinfo SET username=?,department=?,created=?")
	checkError(err)

	res, err := stmt.Exec("windqiu", "developer", "2019-04-01")
	checkError(err)

	id, err := res.LastInsertId()
	checkError(err)
	fmt.Println("id =", id)

	// update
	stmt, err = db.Prepare("UPDATE test.userinfo SET department=? WHERE uid=?")
	checkError(err)
	res, err = stmt.Exec("weishi", id)
	checkError(err)

	affect, err := res.RowsAffected()
	checkError(err)
	fmt.Println("affect =", affect)

}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}
