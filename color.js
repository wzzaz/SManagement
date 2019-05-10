function sFloralWhite() {
    return "#FFFAF0"
}

function sFloralWhite_Half() {
    return "#80FFFAF0"
}

function sNavajoWhite() {
    return "#FFDEAD"
}

function sNavajoWhite_Half() {
    return "#80FFDEAD"
}

function waitingStateColor() {
    return "#00FF7F" //green
}

function unDoneStateColor() {
    return "#CD5C5C" //red
}

function doneStateColor() {
    return "#BEBEBE" //gray
}

function nearPastDueColor() {
    return "#FFA500" //orange
}

function selectionColor() {
    return "#4876FF"
}

function enabledColor() {
    return "#D3D3D3"
}

function stageSelectedColor() {
    return "#00BFFF"
}

function stageResultColor() {
    return "#FFFACD"
}

function stageStatusColor(status) {
    switch(status)
    {
    case 1:
        return doneStateColor()
    case 2:
        return unDoneStateColor()
    case 3:
        return nearPastDueColor()
    case 4:
        return waitingStateColor()
    }
}
