function dateFtt(fmt,date) {
    var o = {
    "M+" : date.getMonth()+1,                 //月份
    "d+" : date.getDate(),                    //日
    "h+" : date.getHours(),                   //小时
    "m+" : date.getMinutes(),                 //分
    "s+" : date.getSeconds(),                 //秒
    "q+" : Math.floor((date.getMonth()+3)/3), //季度
    "S"  : date.getMilliseconds()             //毫秒
    };
    if(/(y+)/.test(fmt))
    fmt=fmt.replace(RegExp.$1, (date.getFullYear()+"").substr(4 - RegExp.$1.length));
    for(var k in o)
    if(new RegExp("("+ k +")").test(fmt))
    fmt = fmt.replace(RegExp.$1, (RegExp.$1.length===1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));
    return fmt;
}

function isInvalidDate(date) {
    return date.toDateString() === ""
}

function dateFormat(fmt, date) {
    if( isInvalidDate(date) ) {
        return ""
    } else {
        return dateFtt(fmt,date)
    }
}

function isEmptyString(val) {
    return val === ""
}

function isDiffString(val1,val2) {
    return val1 !== val2
}

function resNormalPath(name) {
    return "qrc:/res/" + name + ".png"
}

function resHoverPath(name) {
    return "qrc:/res/" + name + "Hover.png"
}

function resPressPath(name) {
    return "qrc:/res/" + name + "Press.png"
}

function overDateForNow(date) {
    var now = new Date
    if( now > date ) {
        return true
    }
    return false
}

function nearThreeDayForNow(date) {
    var now = new Date
    if( now > date - 259200000 ) {  // 1000*60*60*24*3
        return true
    }
    return false
}
