// Global variables
let part1 = "<div class='element bg-danger h-100 rounded d-flex justify-content-center align-items-center'>"
let part2 = "</div>";
let movementsArr = [];
let translationValue;

// start inputs validation functions
function isValidIntialState(state) {
    let regex = /^[0-8]{9}$/;
    let regexRes = regex.test(state);
    if(regexRes) {
        let charSet = new Set();
        for (let i = 0; i < state.length; i++) {
            charSet.add(state[i]);
        }

        if(charSet.size != 9)
            return false
        else
            return true;
    }
    else
        return false;
}
function isValidAlgorithmType(state) {
    let res = document.getElementById("algorithmType").value;
    return res != 0;
}
// end inputs validation functions
// 
// 
// 
// start alerts controlling functions
function hideAllAlerts(){
    document.querySelectorAll(".alert").forEach((ele) =>
    {
        ele.classList.replace("d-block", "d-none");
    })
}
function displayselectedAlert (index) {
    Array.from(document.querySelectorAll(".alert"))[index].classList.replace("d-none", "d-block");
}
// end alerts controlling functions
// 
// 
// 
// start main functions that calls the c++ code
function callSolve()
{
    hideAllAlerts();
    
    let initialState = document.getElementById("initialState").value;
    let algoType = Number(document.getElementById("algorithmType").value);

    let flag = 0;

    if(!isValidIntialState(initialState)) {
        flag = 1;
        displayselectedAlert(0);
    }
    if(!isValidAlgorithmType(algoType)) {
        flag = 1;
        displayselectedAlert(2);
    }

    if(flag)
        return

    let indexOfZero = initialState.indexOf('0');
    let withoutZero = initialState.replace('0', '');
    
    console.log(indexOfZero, withoutZero);

    Module.ccall('solve',
        'null',
        ['number', 'number', 'number'],
        [withoutZero, indexOfZero, algoType]
    );
}
function SolutionPanel(result)
{
    if (result == "unsolvable") {
        displayselectedAlert(1);
    }
    else {
        movementsArr = parseResult(result);
        console.log(movementsArr);
        generateAnimations();
    }
}
function parseResult(result) {
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
    return tempMoves;
}
// end main functions that calls the c++ code
// 
// 
// 
// start the animation controlling functions
function move1(current, direction) {
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
function move2(current, old) {
    let tmp = document.querySelector(`#${current} .element`).innerHTML;
    tmp = part1 + tmp + part2;
    document.querySelector(`#${old}`).innerHTML = `${tmp}`;
    document.querySelector(`#${current}`).innerHTML = "";
}
function animate(current, direction, old) {
    move1(current, direction);
    setTimeout(() => {
        move2(current, old);
    }, 500);
}
function generateAnimations() {
    let count = 0;
    let len = movementsArr.length - 1;
    animate(movementsArr[count].newZero, movementsArr[count].direction, movementsArr[count].currentZero);
    ++count;
    let timeInt = setInterval(() => {
        if(count >= len) {

            clearInterval(timeInt);
        }
        animate(movementsArr[count].newZero, movementsArr[count].direction, movementsArr[count].currentZero);
        count++
    }, 500)
    movementsArr.forEach(async (ele) => {
    })
}
// end the animation controlling functions
// 
// 
// 
// add click event listener to Solve button
document.getElementById("solve").addEventListener("click", () => {
    translationValue = $("#slot0").innerWidth();
    console.time("c++");
    callSolve();
    console.timeEnd("c++");
})