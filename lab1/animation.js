let res = [];
let translationValue;
let part1 = "<div class='element bg-danger h-100 rounded d-flex justify-content-center align-items-center'>"
let part2 = "</div>";

function callSolve()
{
    let a = document.getElementById("t1").value;
    let algoType = Number(document.getElementById("t2").value);

    let indexOfZero = a.indexOf('0');
    let withoutZero = a.replace('0', '');
    
    console.log(indexOfZero, withoutZero);

    Module.ccall('solve',
        'null',
        ['number', 'number', 'number'],
        [withoutZero, indexOfZero, algoType]
    );
}
function SolutionPanel(result)
{
    console.log(result);
    let moves = result.split(",");
    moves.pop();
    console.log(moves);
    let tempMoves = [];
    let tempMove = {};
    let temp;
    moves.forEach((move) => {
        temp = move.split("-");
        console.log(temp);
        tempMove.newZero = temp[0];
        tempMove.direction = temp[1];
        tempMove.currentZero = temp[2];
        tempMoves.push(tempMove);
        tempMove = {}
    })
    res = tempMoves;
    console.log(res);
    // generateAnimations();
    // console.log(result);
}

document.getElementById("tst").addEventListener("click", () => {
    translationValue = $("#slot0").innerWidth();
    console.time("c++");
    callSolve();
    generateAnimations();
    console.timeEnd("c++");
})

function f1(current, direction) {
    let a,b;
    if (direction == "up" || direction == "down")
        a = "Y";
    else
        a = "X";
    if (direction == "up" || direction == "left")
        b = "-";
    else
        b = "";

    let trans = `transform: translate${a}(${b}${translationValue}px);`;

    document.querySelector(`#${current} .element`).style.cssText = trans;
}
function f2(current, old) {
    let tmp = document.querySelector(`#${current} .element`).innerHTML;
    // let tmp = document.querySelector(`#slot1 .element`).innerHTML;
    // tmp = String(tmp);
    // tmp = tmp.toString();
    // tmp = JSON.stringify(tmp);
    // console.log(tmp);
    tmp = part1 + tmp + part2;
    document.querySelector(`#${old}`).innerHTML = `${tmp}`;
    document.querySelector(`#${current}`).innerHTML = "";
    // document.querySelector("#slot0").innerHTML = `${tmp}`;
    // document.querySelector("#slot1").innerHTML = "";
}
function generateAnimations() {
    let count = 0;
    let len = res.length - 1;
    animate(res[count].newZero, res[count].direction, res[count].currentZero);
    ++count;
    let timeInt = setInterval(() => {
        // console.log("enter");
        // console.log(count);
        if(count >= len) {
            // console.log("enterclear");

            clearInterval(timeInt);
        }
        // console.log("will call");
        // console.log(count);
        animate(res[count].newZero, res[count].direction, res[count].currentZero);
        count++
    }, 500)
    res.forEach(async (ele) => {
    })
}

function animate(current, direction, old) {
    f1(current, direction);
    setTimeout(() => {
        f2(current, old);
    }, 500);
    // return new Promise(() => {
    // })
}
// let part1 = "<div class='element bg-danger h-100 rounded d-flex justify-content-center align-items-center'>"
// let part2 = "</div>";

// document.getElementById("tst").addEventListener("click", () => {
//     generateAnimations();
//     // f1();
//     // setTimeout(f2,500);
// })

// function f1(current, direction) {
//     let a,b;
//     if (direction == "up" || direction == "down") {
//         a = "Y";
//     }
//     else
//         a = "X";
//     if (direction == "up" || direction == "left") {
//         b = "-";
//     }
//     else
//         b = "";
//     let trans = `transform: translate${a}(${b}101.328px);`;
//     console.log(trans);
//     document.querySelector(`#${current} .element`).style.cssText = trans;
//     // document.querySelector(`#${current} .element`).style.cssText = `transform: translateX(-101.328px);`
//     // document.querySelector(`#slot1 .element`).style.cssText = "transform: translateX(-101.328px);"
//     // return new Promise(function (resolved) {
//     //     resolved();
//     // })
// }
// function f2(current, old) {
//     let tmp = document.querySelector(`#${current} .element`).innerHTML;
//     // let tmp = document.querySelector(`#slot1 .element`).innerHTML;
//     // tmp = String(tmp);
//     // tmp = tmp.toString();
//     // tmp = JSON.stringify(tmp);
//     console.log(tmp);
//     tmp = part1 + tmp + part2;
//     document.querySelector(`#${old}`).innerHTML = `${tmp}`;
//     document.querySelector(`#${current}`).innerHTML = "";
//     // document.querySelector("#slot0").innerHTML = `${tmp}`;
//     // document.querySelector("#slot1").innerHTML = "";
// }
// let res = [
//     {
//         newZero: "slot1",
//         direction: "left",
//         currentZero: "slot0"
//     },
//     {
//         newZero: "slot4",
//         direction: "up",
//         currentZero: "slot1"
//     },
// ];
// async function generateAnimations() {
//     res.forEach(async (ele) => {
//         await animate(ele.newZero, ele.direction, ele.currentZero);
//     })
// }

// function animate(current, direction, old) {
//     return new Promise(() => {
//         f1(current, direction);
//         setTimeout(() => {
//             f2(current, old);
//         }, 500);
//     })
// }
// *************************************************************************************
// function f1() {
//     return new Promise(function (resolved) {
//         document.querySelector("#slot1 .element").style.cssText = "transform: translateX(-101.328px);"
//         resolved();
//     })
// }
// function f2() {
//     return new Promise(function () {

//         let tmp = document.querySelector("#slot1 .element").innerHTML;
//         // tmp = String(tmp);
//         // tmp = tmp.toString();
//         // tmp = JSON.stringify(tmp);
//         console.log(tmp);
//         tmp = part1 + tmp + part2;
//         document.querySelector("#slot0").innerHTML = `${tmp}`;
//         document.querySelector("#slot1").innerHTML = "";
    
//     })
// }