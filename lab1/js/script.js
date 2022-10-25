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
}
// console.log(Module);