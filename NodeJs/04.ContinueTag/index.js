
// tag1:
// for (let i = 0; i < 3; ++i) {
//     if (i == 1) {
//         break tag1;
//     }
//     console.log("i=", i)
// }
// // output
// // i=0


tag1:
for (let i = 0; i < 3; ++i) {
    if (i == 1) {
        continue tag1;
    }
    console.log("i=", i)
}
// output
// i=0
// i=2
