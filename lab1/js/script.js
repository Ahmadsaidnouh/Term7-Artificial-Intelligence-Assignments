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
let currentlySolvedState = "";
let initialStateInpt = document.getElementById("initialState");
let algorithmType = document.getElementById("algorithmType");
let excTime = document.getElementById("excTime");
let cost = document.getElementById("cost");
let depth = document.getElementById("depth");
let expanded = document.getElementById("expanded");
let excTimeValue;
let costValue;
let depthValue;
let expandedValue;
let errorExists = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start inputs validation functions //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end inputs validation functions ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start alerts, statistics && initial state controlling functions ////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
function resetStatistics()
{
    excTime.innerText = "-----";
    cost.innerText = "-----";
    depth.innerText = "-----";
    expanded.innerText = "-----";
}
function displayStatistics()
{
    excTime.innerText = excTimeValue + " ms";
    cost.innerText = costValue + " moves";
    depth.innerText = depthValue + " nodes";
    expanded.innerText = expandedValue + " nodes";
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end alerts, statistics && initial state controlling functions //////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start main functions that call the c++ code ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function callSolve()
{
    hideAllAlerts();
    errorExists = 0;

    let initialState = initialStateInpt.value;
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

    if (flag || (currentlySolvedState == initialState)) {
        errorExists = 1;
        resetStatistics();
        return;
    }



    currentlySolvedState = initialState;
    console.log("solve");
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
        errorExists = 1;
        displayselectedAlert(1);
    }
    else {
        movementsArr = parseResult(result);
        if (stepMethod) {
            currentStep = 0;
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
    let response = result.split(";");

    let path = response[0];
    expandedValue = response[1];
    depthValue = response[2];

    let moves = path.split(",");
    moves.pop();
    let tempMoves = [];
    let tempMove = {};
    let temp;
    moves.forEach((move) =>
    {
        temp = move.split("-");
        tempMove.newZero = temp[0];
        tempMove.direction = temp[1];
        tempMove.currentZero = temp[2];
        tempMoves.push(tempMove);
        tempMove = {}
    })
    costValue = tempMoves.length;
    return tempMoves;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end main functions that call the c++ code //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start the animation controlling functions //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    if (len == -1)
        return

    animate(movementsArr[count].newZero, movementsArr[count].direction, movementsArr[count].currentZero);
    ++count;
    let timeInt = setInterval(() =>
    {
        if (count > len) {
            clearInterval(timeInt);
        }
        else {

            animate(movementsArr[count].newZero, movementsArr[count].direction, movementsArr[count].currentZero);
            count++
        }
    }, 500)
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end the animation controlling functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start step by step animation control functions//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        animate(movementsArr[currentStep].newZero, movementsArr[currentStep].direction, movementsArr[currentStep].currentZero)
        ++currentStep;
    }
})
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end step by step animation control functions ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// add click event listener to handle various user interactions ///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
solveBtn.addEventListener("click", () =>
{
    translationValue = $("#slot0").innerWidth();
    console.time("c++");
    let t1 = performance.now();
    callSolve();
    excTimeValue = performance.now() - t1;
    console.timeEnd("c++");
    if (!errorExists) {
        displayStatistics();
    }
})
initialStateInpt.addEventListener("keyup", () =>
{
    let state = initialStateInpt.value;
    if (isValidIntialState(state)) {
        displayInitialState(state);
    }
})
initialStateInpt.addEventListener("change", () =>
{
    resetStatistics();
    
    currentStep = 0;
    previousBtn.disabled = true;
    nextBtn.disabled = true;
    
    let state = initialStateInpt.value;
    if (isValidIntialState(state)) {
        currentlySolvedState = "";
        displayInitialState(state);
    }
})
algorithmType.addEventListener("change", () =>
{
    resetStatistics();
    
    currentStep = 0;
    previousBtn.disabled = true;
    nextBtn.disabled = true;

    currentlySolvedState = "";
    let state = initialStateInpt.value;
    if (isValidIntialState(state)) {
        displayInitialState(state);
    }
})