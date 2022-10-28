// Global variables
let part1 = "<div class='element bg-danger h-100 d-flex justify-content-center align-items-center'>"
let part2 = "</div>";
let movementsArr = [];
let translationValue;
let stepMethodcheckBox = document.getElementById("stepMethod");
let solveBtn = document.getElementById("solve");
let previousBtn = document.getElementById("prev");
let nextBtn = document.getElementById("next");
let stepMethod = false;
let currentStep = 0;

// start inputs validation functions
function isValidIntialState(state)
{
    let regex = /^[0-8]{9}$/;
    let regexRes = regex.test(state);
    if (regexRes) {
        let charSet = new Set();
        for (let i = 0; i < state.length; i++) {
            charSet.add(state[i]);
        }

        if (charSet.size != 9)
            return false
        else
            return true;
    }
    else
        return false;
}
function isValidAlgorithmType(state)
{
    let res = document.getElementById("algorithmType").value;
    return res != 0;
}
// end inputs validation functions
// 
// 
// 
// start alerts controlling functions
function hideAllAlerts()
{
    document.querySelectorAll(".alert").forEach((ele) =>
    {
        ele.classList.replace("d-block", "d-none");
    })
}
function displayselectedAlert(index)
{
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

    if (!isValidIntialState(initialState)) {
        flag = 1;
        displayselectedAlert(0);
    }
    if (!isValidAlgorithmType(algoType)) {
        flag = 1;
        displayselectedAlert(2);
    }

    if (flag)
        return

    initialState = Number(initialState);

    Module.ccall('solve',
        'null',
        ['number', 'number'],
        [initialState, algoType]
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
        if (stepMethod) {
            previousBtn.disabled = false;
            nextBtn.disabled = false;
        }
        else {
            generateAnimations();
        }
    }
}
function parseResult(result)
{
    console.log(result);
    let moves = result.split(",");
    moves.pop();
    console.log(moves);
    let tempMoves = [];
    let tempMove = {};
    let temp;
    moves.forEach((move) =>
    {
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
function move1(current, direction)
{
    let a, b;
    if (direction == "up" || direction == "down")
        a = "Y";
    else
        a = "X";
    if (direction == "up" || direction == "left")
        b = "-";
    else
        b = "";

    let trans = `transform: translate${a}(${b}${translationValue}px);`;

    document.querySelector(`#slot${current} .element`).style.cssText = trans;
}
function move2(current, old)
{
    let tmp = document.querySelector(`#slot${current} .element`).innerHTML;
    tmp = part1 + tmp + part2;
    document.querySelector(`#slot${old}`).innerHTML = `${tmp}`;
    document.querySelector(`#slot${current}`).innerHTML = "";
}
function animate(current, direction, old)
{
    move1(current, direction);
    setTimeout(() =>
    {
        move2(current, old);
    }, 500);
}
function generateAnimations()
{
    let count = 0;
    let len = movementsArr.length - 1;
    animate(movementsArr[count].newZero, movementsArr[count].direction, movementsArr[count].currentZero);
    ++count;
    let timeInt = setInterval(() =>
    {
        if (count >= len) {
            clearInterval(timeInt);
        }
        animate(movementsArr[count].newZero, movementsArr[count].direction, movementsArr[count].currentZero);
        count++
    }, 500)
}
function displayInitialState(state)
{
    let tmp;
    for (let i = 0; i < state.length; i++) {
        if (state[i] == "0") {
            document.querySelector(`#slot${i}`).innerHTML = "";
        }
        else {
            tmp = part1 + state[i] + part2;
            document.querySelector(`#slot${i}`).innerHTML = `${tmp}`;
        }

    }
}
// end the animation controlling functions
// 
// 
// 
// start step by step animation control functions
stepMethodcheckBox.addEventListener("click", function ()
{
    if (stepMethodcheckBox.checked) {
        stepMethod = true;
    }
    else {
        previousBtn.disabled = true;
        nextBtn.disabled = true;
        stepMethod = false;
    }
})
previousBtn.addEventListener("click", () =>
{
    if (currentStep > 0) {
        // console.log(movementsArr[currentStep]);
        let oppositeDirection;

        if (movementsArr[currentStep - 1].direction == "up")
            oppositeDirection = "down";
        else if (movementsArr[currentStep - 1].direction == "down")
            oppositeDirection = "up";
        else if (movementsArr[currentStep - 1].direction == "left")
            oppositeDirection = "right";
        else if (movementsArr[currentStep - 1].direction == "right")
            oppositeDirection = "left";

        animate(movementsArr[currentStep - 1].currentZero, oppositeDirection, movementsArr[currentStep - 1].newZero)
        --currentStep;
    }
})
nextBtn.addEventListener("click", () =>
{
    if (currentStep < movementsArr.length) {
        // console.log(movementsArr[currentStep]);
        animate(movementsArr[currentStep].newZero, movementsArr[currentStep].direction, movementsArr[currentStep].currentZero)
        ++currentStep;
    }
})
// end step by step animation control functions
// 
// 
// 
// add click event listener to Solve button
solveBtn.addEventListener("click", () =>
{
    translationValue = $("#slot0").innerWidth();
    console.time("c++");
    callSolve();
    console.timeEnd("c++");
})
// document.getElementById("initialState").addEventListener("keyup", () => {
//     let state = document.getElementById("initialState").value;
//     if(isValidIntialState(state))
//     {
//         displayInitialState(state);
//     }
// })