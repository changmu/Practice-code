package main

type Dollar struct {
	amount int
}

func (d *Dollar) Times(n int) Dollar {
	return Dollar{
		amount: n * 2,
	}
}
