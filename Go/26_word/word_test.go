package word

import "testing"

func TestPalindrome(t *testing.T) {
    if !IsPalindrome("aba") {
        t.Error(`IsPalindrome("aba") = false`)
    }
    if !IsPalindrome("xyx") {
        t.Error(`IsPalindrome("xyx") = false`)
    }
}

func TestNonPalindrome(t *testing.T) {
    if IsPalindrome("abc") {
        t.Error(`IsPalindrome("abc") = true`)
    }
}
