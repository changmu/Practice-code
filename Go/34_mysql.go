package main

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
	_ "github.com/mattn/go-sqlite3"
)

func main() {
	// db, err := sql.Open("mysql", "qiu:@tcp(192.168.126.10:3306)/?charset=utf8")
	db, err := sql.Open("sqlite3", "./sqlite.db")
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

	// query
	rows, err := db.Query("SELECT * FROM test.userinfo")
	checkError(err)

	for rows.Next() {
		uid, username, department, created := 0, "", "", ""
		err = rows.Scan(&uid, &username, &department, &created)
		checkError(err)

		fmt.Printf("row: %v,%v,%v,%v\n", uid, username, department, created)
	}

	// delete
	stmt, err = db.Prepare("DELETE FROM test.userinfo where uid>?")
	checkError(err)

	res, err = stmt.Exec(3)
	// fmt.Printf("res=%v\n", res)
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}
