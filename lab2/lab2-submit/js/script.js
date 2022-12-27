// Global variables
let firstPlay = true;
let algorithmType = document.getElementById("algorithmType");
let depthInput = document.getElementById("depthIp");
let message = document.getElementById("message");
let algorithm;
let turn = 0;
let tState = "222222222222222222222222222222222222222222";
let tPastState = "222222222222222222222222222222222222222222";
let emptySlots = 42;
let games = 42;
let depth;
let state = [];
let pastState = [];
let nodes = []
let tree = {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start actions controlling functions  ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
$(".slot").click((e) =>
{
    if (emptySlots == 0) {
        return;
    }

    // validates that inputs exists
    if ((algorithmType.value == 0 || depthInput.value == 0) && firstPlay) {
        if (depthInput.value == 0)
            Array.from(document.querySelectorAll(".alert"))[0].classList.replace("d-none", "d-block");
        if (algorithmType.value == 0)
            Array.from(document.querySelectorAll(".alert"))[1].classList.replace("d-none", "d-block");
        return;
    }
    else if (firstPlay) {
        Array.from(document.querySelectorAll(".alert"))[0].classList.replace("d-block", "d-none");
        Array.from(document.querySelectorAll(".alert"))[1].classList.replace("d-block", "d-none");
        firstPlay = false;
        algorithm = algorithmType.value;
        depth = Number(document.getElementById("depthIp").value);
    }

    let targetSlot = e.target;
    if (!targetSlot.classList.contains("filled") && !targetSlot.classList.contains("invalid")) {
        if (turn == 0) {
            tPastState = tState
            targetSlot.style.cssText = "background-color: red;";
            tState = "";
            for (let j = 0; j < 7; j++) {
                for (let i = 5; i >= 0; i--) {
                    let id = `slot${i}${j}`;
                    let slotBC = document.getElementById(id).style.backgroundColor;
                    if (slotBC == 'red') tState += '0';
                    else if (slotBC == 'yellow') tState += '1';
                    else {
                        tState += '2';
                    }
                }

            }
            state = [];
            state.push(Number(tState.slice(33)))
            tState = tState.slice(0, 33)
            state.push(Number(tState.slice(24)))
            tState = tState.slice(0, 24)
            state.push(Number(tState.slice(15)))
            tState = tState.slice(0, 15)
            state.push(Number(tState.slice(6)))
            tState = tState.slice(0, 6)
            state.push(Number(tState))
            // **************
            pastState = [];
            pastState.push(Number(tPastState.slice(33)))
            tPastState = tPastState.slice(0, 33)
            pastState.push(Number(tPastState.slice(24)))
            tPastState = tPastState.slice(0, 24)
            pastState.push(Number(tPastState.slice(15)))
            tPastState = tPastState.slice(0, 15)
            pastState.push(Number(tPastState.slice(6)))
            tPastState = tPastState.slice(0, 6)
            pastState.push(Number(tPastState))

        }
        targetSlot.classList.add("filled");
        turn = !turn;

        let row = Number(targetSlot.id[4]) + 1;
        if (row < 6) {
            let newId = `slot${row}${targetSlot.id[5]}`
            document.getElementById(newId).classList.remove("invalid");

        }
        games--;
        console.time("c++");
        callSolve();
        console.timeEnd("c++");
    }
})
// decide the winner
function calcWinner()
{
    let finalState = [];
    for (let j = 0; j < 7; j++) {
        let t = []
        for (let i = 5; i >= 0; i--) {
            let id = `slot${i}${j}`;
            let slotBC = document.getElementById(id).style.backgroundColor;
            if (slotBC == 'red') t.push(0);
            else if (slotBC == 'yellow') t.push(1);
            else {
                t.push(2);
            }
        }
        finalState.push(t);
    }
    let myScore = 0, AIScore = 0;
    // vertical
    for (let i = 0; i < 7; i++) {
        for (let j = 0; j < 3; j++) {
            let p1 = finalState[i][j]
            let p2 = finalState[i][j + 1]
            let p3 = finalState[i][j + 2]
            let p4 = finalState[i][j + 3]


            if ((p1 === 0) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                myScore++
            }
            if ((p1 === 1) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                AIScore++

            }


        }
    }
    // horizontal
    for (let i = 0; i < 6; i++) {
        for (let j = 0; j < 4; j++) {
            let p1 = finalState[j][i]
            let p2 = finalState[j + 1][i]
            let p3 = finalState[j + 2][i]
            let p4 = finalState[j + 3][i]


            if ((p1 === 0) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                myScore++
            }
            if ((p1 === 1) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                AIScore++

            }


        }
    }
    // diag2
    for (let i = 6; i > 2; i--) {
        for (let j = 0; j < 3; j++) {
            let p1 = finalState[i][j]
            let p2 = finalState[i - 1][j + 1]
            let p3 = finalState[i - 2][j + 2]
            let p4 = finalState[i - 3][j + 3]


            if ((p1 === 0) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                myScore++
            }
            if ((p1 === 1) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                AIScore++

            }


        }
    }
    // diag1
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 3; j++) {
            let p1 = finalState[i][j]
            let p2 = finalState[i + 1][j + 1]
            let p3 = finalState[i + 2][j + 2]
            let p4 = finalState[i + 3][j + 3]


            if ((p1 === 0) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                myScore++
            }
            if ((p1 === 1) && (p1 === p2) && (p2 === p3) && (p3 == p4)) {
                AIScore++

            }


        }
    }
    console.log(myScore, AIScore);
    if (myScore > AIScore)
        message.innerHTML = "You are the WINNER"
    else if (AIScore > myScore)
        message.innerHTML = "AI is the WINNER"
    else
        message.innerHTML = "Draw"

    message.classList.replace("d-none", "d-block");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end actions controlling functions  /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start main functions that call the c++ code ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function callSolve()
{
    Module.ccall('solve',
        'null',
        ['number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number'],
        [state[4], state[3], state[2], state[1], state[0], pastState[4], pastState[3], pastState[2], pastState[1], pastState[0], Number(algorithm), depth, emptySlots]
    );
}
function SolutionPanel(result)
{
    games--;
    let nodes = result.split('|');
    let axis = nodes[0].split(",");
    document.getElementById(`slot${axis[0]}${axis[1]}`).style.cssText = "background-color: yellow;";
    turn = !turn;
    document.getElementById(`slot${axis[0]}${axis[1]}`).classList.add("filled");
    let row = Number(document.getElementById(`slot${axis[0]}${axis[1]}`).id[4]) + 1;
    if (row < 6) {
        let newId = `slot${row}${document.getElementById(`slot${axis[0]}${axis[1]}`).id[5]}`
        document.getElementById(newId).classList.remove("invalid");
    }
    if (games == 0) {
        calcWinner();
    }

    parseResult(result);
}
function parseResult(result)
{
    let tNodes = result.split("|");
    tNodes.shift();
    tNodes.pop();
    console.log(tNodes.length);
    let root = tNodes[0].split(';')[0];

    let temp
    if (algorithm == 1) {
        tNodes.forEach((n) =>
        {
            temp = n.split(";");
            temp.pop()
            let neig = [];
            for (let i = 0; i < temp.length - 5; i++) {
                neig.push(temp[i + 3]);

            }
            nodes.push({
                state: temp[0],
                score: temp[1],
                action: temp[2],
                neighbors: neig
            })
            tree[temp[0]] = {
                state: temp[0],
                score: temp[1],
                action: temp[2],
                neighbors: neig
            }
        })
    }
    else {
        tNodes.forEach((n) =>
        {
            temp = n.split(";");
            temp.pop()
            let neig = [];
            for (let i = 0; i < temp.length - 5; i++) {
                neig.push(temp[i + 5]);

            }
            nodes.push({
                state: temp[0],
                alpha: temp[1],
                beta: temp[2],
                score: temp[3],
                action: temp[4],
                neighbors: neig
            })
            tree[temp[0]] = {
                state: temp[0],
                alpha: temp[1],
                beta: temp[2],
                score: temp[3],
                action: temp[4],
                neighbors: neig
            }
        })
    }
    
    displayTree(root)
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end main functions that call the c++ code //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start the tree creation controlling function ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const renderTree = (node) =>
{
    const { state, alpha, beta, neighbors, score, action } = node;
    let cartona = ``;
    if (neighbors.length > 0)
        neighbors.forEach(neighbor =>
        {
            cartona += (`<div class="node">` + renderTree(tree[neighbor]) + `</div>`);
        });
    return `
    ${algorithm == 1 ? `<div class="node__element">State:<br>${state}<br>Score:${score}<br>Action:<br>${action}</div>` : `<div class="node__element">State:<br>${state}<br>a:${alpha} B:${beta} Score:${score}<br>Action:<br>${action}</div>`}
    
    ${neighbors.length > 0 ?
            `
        <div class="node__bottom-line"></div>
        <div class="node__children">
            ${cartona
            }
        </div>
        `
            : ''
        }
    
    `
}
const displayTree = (root) =>
{
    const rootNode = tree[root];
    const treeDOMElement = document.getElementById("root");
    treeDOMElement.innerHTML = renderTree(rootNode);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end the tree creation controlling function ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////