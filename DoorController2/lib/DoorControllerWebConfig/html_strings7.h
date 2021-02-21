#ifndef HTML_STRINGS_H
#define HTML_STRINGS_H
#include <Arduino.h>


const char styling[] PROGMEM = R"rawliteral(
<style>
    body {
    margin: 0;
    font-family: Helvetica, Arial, sans-serif;
    background: #fafafa;
}

.topnav {
    overflow: hidden;
    background-color: #333;
    height: 4vh;
    margin-bottom: 2vh;
}

.topnav a {
    float: left;
    color: #f2f2f2;
    text-align: center;
    padding: 1vh;
    text-decoration: none;
    font-size: 1.8vh;
}

.topnav a:hover {
    background-color: #ddd;
    color: black;
}

.topnav a.active {
    background-color: #559BCC;
    color: white;
}

.pseudologo {
    background-color: #000000;
    color: white;
    font-weight: bold;
}

.container {
    margin: 0 auto;
    width: 100%;
}

.container .col {
    background: lightblue;
}

.container .col p {
    padding: .25rem .75rem;
}


.submitBtn {
    background-color: #1E90FF;
    border-radius: 5px;
    border: 0;
    display: inline-block;
    cursor: pointer;
    color: #ffffff;
    font-family: Arial;
    text-decoration: none;
    width: 30vh;
    height: 5vh;
}

.submitBtn:hover {
    background-color: #559BCC;
}

.submitBtn:active {
    position: relative;
    top: 1px;
}
.resetBtn {
    background-color: #FF8000;
    border-radius: 5px;
    border: 0;
    display: inline-block;
    cursor: pointer;
    color: #ffffff;
    font-family: Arial;
    text-decoration: none;
    width: 30vh;
    height: 5vh;
}

.resetBtn:hover {
    background-color: #FF7F50;
}

.resetBtn:active {
    position: relative;
    top: 1px;
}

.defaultBtn {
    background-color: #FF3333;
    border-radius: 5px;
    border: 0;
    display: inline-block;
    cursor: pointer;
    color: #ffffff;
    font-family: Arial;
    text-decoration: none;
    width: 30vh;
    height: 5vh;
}

.dafaultBtn:hover {
    background-color: #CD5C5C;
}

.dafaultBtn:active {
    position: relative;
    top: 1px;
}

input[type=text],
input[type=password] {
    width: 25%;
    padding: 0.9vh;
    height: 0.8vh;
    margin-bottom: 1.5vh;
}

input[type=checkbox]+label {
    font-size: 1.5vh;
    margin-top: 0.1em;
}

.selector {
    width: 26.5%;
    height: 3.3vh;
    margin-bottom: 1.5vh;
}

.dropdown {
    display: none;
}

.footer {
    position: fixed;
    right: 0;
    bottom: 0;
    left: 0;
    padding: 0.5vh;
    font-size: 2vh;
    background-color: #333;
    text-align: center;
    color: white;
}

.showPwLeft{
    position: absolute;
    margin-left: 37%;
}

@media screen and (max-width: 640px) {
    
    .showPwLeft {
        position: absolute;
        margin-left: 22%;
    }
    .topnav {
        display: none !important;
    }

    .dropdown {
        position: relative;
        display: inline-block !important;
        width: 100%;
    }

    .dropbtn {
        background-color: #559BCC;
        color: white;
        margin-top: 0px;
        padding: 1.5vh;
        border: none;
        width: 100%;
        font-size: 4vh;
    }

    .dropdown-content {
        display: none;
        width: 100%;
        padding-top: 0px;
        position: absolute;
        background-color: #333;
        min-width: 50px;
        overflow: auto;
        z-index: 1;
        font-size: 2vh;
    }

    .dropdown-content a {
        color: white;
        padding: 1em;
        text-decoration: none;
        display: block;
    }

    .show {
        display: block;
    }

    .formUpdate {
        width: 70%;
        border-radius: 15px;
    }

    input[type=text],
    input[type=password] {
        width: 50% !important;
        padding: 0.9vh;
        height: 0.8vh;
    }

    input[type=checkbox]+label {
        font-size: 1.5vh;
        vertical-align: center;
        margin-top: 0.1em;
    }

    .selector {
        width: 55% !important;
        height: 3.3vh;
        margin-top: 0.4vh;
    }
}
@media only screen and (min-width:600px) {
    .container .col {
        float: left;
        width: 50%;
    }
}
@media only screen and (min-width:768px) {
    .container .col {
        width: 33.333%;
    }
}

@media only screen and (min-width:992px) {
    .container .col {
        width: 25%;
    }
}
</style>
</html>
)rawliteral";

/////////////////////////////////////////////////////////////////////// Login page ///////////////////////////////////////////////////////////////////////
String APloginpage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>LIDARino Login</title>
    <style type="text/css">
        body {
            margin: 0;
            font-family: Helvetica, Arial, sans-serif;
            background: #fafafa;
        }
        input[type=text],
        input[type=password] {
            width: 90%;
            padding: 1vw 1vw;
            margin: 0.5vw 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
            }
        .login-form {
            width: 50vh;
            margin: 5vh auto;
        }
        .login-form form {
            background: #f7f7f7;
            box-shadow: 0px 2px 2px rgba(0, 0, 0, 0.3);
            border-radius: 1vh;
            padding: 2vw;
        }
        .button7 {
            display: inline-block;
            padding: 0.7em 1.7em;
            margin: 0 0.3em 0.3em 0;
            border-radius: 0.8em;
            border: solid #fafafa;
            box-sizing: border-box;
            text-decoration: none;
            color: #FFFFFF;
            background-color: #1E90FF;
            box-shadow: inset 0 -0.6em 1em -0.35em rgba(0, 0, 0, 0.17), inset 0 0.6em 2em -0.3em rgba(255, 255, 255, 0.15), inset 0 0 0em 0.05em rgba(255, 255, 255, 0.12);
            text-align: center;
            position: relative;
            width: 25vh;
        }
        .button7:active {
            box-shadow: inset 0 0.6em 2em -0.3em rgba(0, 0, 0, 0.15), inset 0 0 0em 0.05em rgba(255, 255, 255, 0.12);
        }
        .button7:hover {
            box-shadow: inset 0 0.6em 2em -0.3em rgba(0, 0, 0, 255), inset 0 0 0em 0.05em rgba(255, 255, 255, 0.12);
        }
        .showPwLeft{
            position: absolute;
            margin-left: 1%;
        }
        @media all and (max-width:640px) {
            .showPwLeft{
                position: absolute;
                margin-left: 2.2%;
            }
            .button7 {
                display: block;
                width: 40vw;
            }
        
            .login-form {
                width: 80vw;
                margin: 5vh auto;
            }
        }
    </style>
</head>

<body onload="deleteAllCookies();">
    <div class="login-form">
        <form action="/dashboard" method="POST">
            <center><img src=" data:image/png;base64,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"
                    class="img-fluid align-center" alt="Qibixx LIDARino" style="height: auto; width:45vmin"></center>
            <center><div style="width: auto;" class="form-group">
                <input type="text" name="user" class="form-control" placeholder="Username" required="required"><br>
                <input type="password" name="pass" id="pswInp" class="form-control" placeholder="Password" required="required"><br>
                <div class="showPwLeft"><input type="checkbox" id="check" onclick="showPass()"><label for="check">Show Password</label></div><br>
                <br><button type="submit" class="button7">LOGIN</button>
            </div></center>
        </form>
    </div>
</body>
<script>
    function deleteAllCookies() {
        var cookies = document.cookie.split(";");
        for (var i = 0; i < cookies.length; i++) {
            var cookie = cookies[i];
            var eqPos = cookie.indexOf("=");
            var name = eqPos > -1 ? cookie.substr(0, eqPos) : cookie;
            document.cookie = name + "=;expires=Thu, 01 Jan 1970 00:00:00 GMT";
        }
    }
    function showPass() {
        var x = document.getElementById("pswInp");
        if (x.type === "password") {
            x.type = "text";
        } else {
            x.type = "password";
        }
    }
</script>
</html>
)rawliteral";

/////////////////////////////////////////////////////////////////////// AP DASHBOARD ///////////////////////////////////////////////////////////////////////
String AP_dash_html=R"rawliteral(
<!DOCTYPE html>
<html>
<head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Qibixx Dashboard</title
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
</head>
<body>
    <div class="dropdown">
        <button onclick="showDropdown()" class="dropbtn">Grimpa Door Controller &nbsp;&#9776;</button>
        <div id="myDropdown" class="dropdown-content">
            <a class="pseudologo">Home</a>
            <a href="/pushSettings">Push Settings</a>
            <a href="/update">Update</a>
            <a href="/wifi">WiFi</a>
            <a href="/credentials">Credentials</a>
            <a href="/control">Control</a>
            <a href="/">Sign Out</a>
        </div>
    </div>

    <div class="topnav">
        <a class="active">Grimpa Door Controller</a>
        <a class="pseudologo">Home</a>
        <a href="/pushSettings">Push Settings</a>
        <a href="/update">Update</a>
        <a href="/wifi">Network</a>
        <a href="/credentials">Credentials</a>
        <a href="/control">Control</a>
        <a href="/">Sign Out</a>
    </div>

    <div class="container">
        <center>
            <form id="settingsForm" onsubmit="return readForm();" action="/ChangeSettings" method="get">
                <div style="text-align:center;">
                    <label for="uname">Network Status</label><br>
                    <input type="text" id="statusNet" value="%PLACEHOLDER_NETWORK_STATUS%" id="netStatus" name="ignore" readonly><br>
                    <label for="macaddr">WiFi Mac Address</label><br>
                    <input type="text" placeholder="%PLACEHOLDER_MAC%" name="ignore" id="macAdd" readonly><br>
                    <label for="macaddrEth" %PLACEHOLDER_HIDDEN_ETH%>Ethernet Mac Address</label><br %PLACEHOLDER_HIDDEN_ETH%>
                    <input type="text" placeholder="%PLACEHOLDER_MAC_ETH%" name="ignore" readonly %PLACEHOLDER_HIDDEN_ETH%><br %PLACEHOLDER_HIDDEN_ETH%>
                    <label for="motorOpenTime">Motor Open/Close Timeout (ms)</label><br>
                    <input type="text" value="%PLACEHOLDER_MOTOR_TIMEOUT%" name="motorOpenTime"><br>
                </div>
                <br>
                <div class="container" style="text-align:center;">
                    <button class="submitBtn">Submit Changes</button><br><br>
                    <button class="resetBtn" onclick="doGetRequest('/reset'); return false;" class="submitBtn">Reboot Device</button><br><br>
                    <button class="defaultBtn" onclick="return resetDefault()">Reset to Defaults <span>&#9888;</span></button>
                </div>
        </center>
        </form>
    </div>

</body>
<div class="footer">%PLACEHOLDER_SWVERSION%</div>
<script>

    function showDropdown() {
        document.getElementById("myDropdown").classList.toggle("show");
    }
    window.onclick = function (event) {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    window.ontouchend = function (event) {
        setTimeout(timedFunc, 500);
    }

    function timedFunc() {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    function resetDefault() {
        var r = confirm("This operation will reset the device to default configuration!\n Do you want to proceed?");
        if (r == true) {
            doGetRequest('/FactoryReset');
            return false;
        }
        else return false;
    }

    function readForm(formName = "settingsForm") {
        let formData = Array.from(document.querySelectorAll('#' + formName + ' input, select')).reduce((acc, input) => ({ ...acc, [input.name]: input.value }), {});
        let action = document.getElementById(formName).action;
        let params = '?';

        for (name in formData) params += name + '=' + formData[name] + '&';

        let urltoSend = action.concat(params).slice(0, -1);
        console.log(urltoSend);

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) alert(xhttp.responseText);
        };

        xhttp.open("GET", urltoSend, true);
        xhttp.send();

        return false;
    }

    function doGetRequest(endpoint) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(xhttp.responseText);
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

</script>
)rawliteral";



/////////////////////////////////////////////////////////////////////// FIRMWARE UPDATE ///////////////////////////////////////////////////////////////////////
String AP_firmware=R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Qibixx Firmware Update</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    </head>
<body>
    <div class="dropdown">
        <button onclick="showDropdown()" class="dropbtn">Grimpa Door Controller &nbsp;&#9776;</button>
        <div id="myDropdown" class="dropdown-content">
            <a href="/dashboard">Home</a>
            <a href="/pushSettings">Push Settings</a>
            <a class="pseudologo">Update</a>
            <a href="/wifi">Network</a>
            <a href="/credentials">Credentials</a>
            <a href="/control">Control</a>
            <a href="/">Sign Out</a>
        </div>
    </div>
    <div class="topnav">
        <a class="active">Grimpa Door Controller</a>
        <a href="/dashboard">Home</a>
        <a href="/pushSettings">Push Settings</a>
        <a class="pseudologo">Update</a>
        <a href="/wifi">Network</a>
        <a href="/credentials">Credentials</a>
        <a href="/control">Control</a>
        <a href="/">Sign Out</a>
    </div>
    <br>
    <div align="center">
        <form method='POST' action='/doUpdate' class="formUpdate" enctype='multipart/form-data'>
            <br><input type='file' name='update'><br><br>
            <center><input class="submitBtn" type='submit' value='Update'></center><br>
        </form>
    </div>
</body>
<div class="footer">%PLACEHOLDER_SWVERSION%</div>
<script>
    function showDropdown() {
        document.getElementById("myDropdown").classList.toggle("show");
    }

    window.onclick = function(event) {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    window.ontouchend = function(event) {
        setTimeout(timedFunc, 500);
    }
    function timedFunc(){
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
</script>
)rawliteral";

/////////////////////////////////////////////////////////////////////// WIFI Connect ///////////////////////////////////////////////////////////////////////
String AP_wificon=R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Qibixx Network</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
</head>
<body onload=" setInitialCfg(); changeWifiMode(); showAdvanced(); showPass('wpa2Psw', 'check'); showPass('pswInp', 'check1');">
    <div class="dropdown">
        <button onclick="showDropdown()" class="dropbtn">Grimpa Door Controller &nbsp;&#9776;</button>
        <div id="myDropdown" class="dropdown-content">
            <a href="/dashboard">Home</a>
            <a href="/pushSettings">Push Settings</a>
            <a href="/update">Update</a>
            <a class="pseudologo">Network</a>
            <a href="/credentials">Credentials</a>
            <a href="/control">Control</a>
            <a href="/">Sign Out</a>
        </div>
    </div>
    <div class="topnav">
        <a class="active">Grimpa Door Controller</a>
        <a href="/dashboard">Home</a>
        <a href="/pushSettings">Push Settings</a>
        <a href="/update">Update</a>
        <a class="pseudologo">Network</a>
        <a href="/credentials">Credentials</a>
        <a href="/control">Control</a>
        <a href="/">Sign Out</a>
    </div><center>
        <form id="settingsForm" onsubmit="return readForm();" action="/WifiConnect" method="GET">
            <label for="netmode" %PLACEHOLDER_HIDDEN_ETH%>Network Interface</label><br %PLACEHOLDER_HIDDEN_ETH%>
            <select  onchange="onNetInterfaceChange();" class="selector" id="netmode" name="netmode" %PLACEHOLDER_HIDDEN_ETH%>
                <option value="%PLACEHOLDER_NETMODE_ACTIVE%"> %PLACEHOLDER_NETMODE_ACTIVE% </option>
                <option value="%PLACEHOLDER_NETMODE_INACTIVE%"> %PLACEHOLDER_NETMODE_INACTIVE% </option>
                <option value="%PLACEHOLDER_NETMODE_INACTIVE2%"> %PLACEHOLDER_NETMODE_INACTIVE2% </option>
            </select><br %PLACEHOLDER_HIDDEN_ETH%>
            <div id="wifiStuff">
                <label>WiFi Security</label><br>
                <select class="selector" name="wifiMode" id="wifiMode" onchange="changeWifiMode()">
                    <option value="wpa_p"> WPA/WPA-2 Personal (Recommended) </option>
                    <option value="wpa_e"> WPA/WPA-2 Enterprise</option>
                </select>
                <div class="container">
                    <label for="uname">WiFi SSID</label><br>
                    <input type="text" value="%PLACEHOLDER_SSID%" name="ssid"><br>
                    <div id="pswselect">
                        <label for="psw">WiFi Password</label><br>
                        <input type="password" style="margin-bottom: 0;" value="%PLACEHOLDER_PSW%" id="pswInp" name="psw"><br>
                        <div class="showPwLeft"><input type="checkbox" id="check1" onclick="showPass('pswInp', 'check1')">
                            <label for="check">Show Password</label>
                        </div><br><br>
                    </div>
                    <div id="enterprise" style="display: none">
                        <label>Username</label><br>
                        <input type="text" value="%PLACEHOLDER_WPA2_UNAME%" id="wpa2Uname" name="wpa2Uname"><br>
                        <label>Password</label><br>
                        <input style="margin-bottom: 0;" type="text" value="%PLACEHOLDER_WPA2_PSW%" id="wpa2Psw" name="wpa2Psw">
                        <div class="showPwLeft"><input type="checkbox" id="check" onclick="showPass('wpa2Psw', 'check')">
                            <label for="check">Show Password</label><br>
                        </div><br><br>
                        <label>Anonymous Identity</label><br>
                        <input type="text" value="%PLACEHOLDER_WPA2_ANONYMOUS_ID%" name="wpa2AnId"><br>
                    </div>
                </div>
            </div>
            <div class="showPwLeft">
                <input type="checkbox" id="advancedopt" onclick="showAdvanced()"> <label for="advancedopt">Show Advanced Options</label></div><br><br>
                <div class="divlabel" id="advanced" style="display: none;">
                    <label for="ip">Static IP address</label><br>
                    <input type="text" value="%PLACEHOLDER_IP%" id="ip" name="ip"><br>
                    <label for="gate">Default Gateway</label><br>
                    <input type="text" value="%PLACEHOLDER_GATEWAY%" name="gateway"><br>
                    <label for="gate">Subnet Mask</label><br>
                    <input type="text" value="%PLACEHOLDER_SUBNET%" name="subnet"><br>
                </div>
                <button class="submitBtn" type="submit">Submit Changes</button>
            </div>
        </form>
    </div>
</center>
    
</body>
<div class="footer">%PLACEHOLDER_SWVERSION%</div>
<script>

    function setInitialCfg(){
        let myCheck = document.getElementById("advancedopt");
        document.getElementById("ip").value == "" ? myCheck.checked = false : myCheck.checked = true;
        let select = document.getElementById("wifiMode");
        if (document.getElementById("wpa2Uname").value!=""){
            select.options[1].selected = "selected";
        }
    }

    function changeWifiMode() {
        if (document.getElementById("wifiMode").value == "wpa_p") {
            document.getElementById("pswselect").style.display = "block";
            document.getElementById("enterprise").style.display = "none";
        }else{
            document.getElementById("pswselect").style.display = "none";
            document.getElementById("enterprise").style.display = "block";
        }
    }

    function onNetInterfaceChange(){
        let wifiDiv = document.getElementById("wifiStuff");
        document.getElementById("netmode").value=="Ethernet" ? wifiDiv.style.display = "none" : wifiDiv.style.display = "block";
    }

    function showPass(inputId, checkboxId) {
        let x = document.getElementById(inputId);
        document.getElementById(checkboxId).checked ? x.type = "text" : x.type="password"
    }

    function showAdvanced() {
        let x = document.getElementById("advanced");
        document.getElementById("advancedopt").checked ? x.style.display = "block" : x.style.display = "none";
    }

    function showDropdown() {
        document.getElementById("myDropdown").classList.toggle("show");
    }

    window.onclick = function (event) {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    window.ontouchend = function (event) {
        setTimeout(timedFunc, 500);
    }

    function timedFunc() {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {
                    openDropdown.classList.remove('show');
                }
            }
        }
    }

    function readForm(formId = "settingsForm") {
        let formData = Array.from(document.querySelectorAll('#' + formId + ' input, select')).reduce((acc, input) => ({ ...acc, [input.name]: input.value }), {});
        let action = document.getElementById(formId).action;
        let params = '?';
        for (name in formData) params += name + '=' + formData[name] + '&';
        let urltoSend = action.concat(params).slice(0, -1);

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) alert(xhttp.responseText);
        };
        console.log(formData);
        console.log(urltoSend);
        xhttp.open("GET", urltoSend, true);
        xhttp.send();
        return false;
    }
</script>
)rawliteral";

/////////////////////////////////////////////////////////////////////// Access Credentials ///////////////////////////////////////////////////////////////////////
String AP_credentials=R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Qibixx Credentials</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    </head>
<body>
    <div class="dropdown">
        <button onclick="showDropdown()" class="dropbtn">Grimpa Door Controller &nbsp;&#9776;</button>
        <div id="myDropdown" class="dropdown-content">
            <a href="/dashboard">Home</a>
            <a href="/pushSettings">Push Settings</a>
            <a href="/update">Update</a>
            <a href="/wifi">Network</a>
            <a class="pseudologo">Credentials</a>
            <a href="/control">Control</a>
            <a href="/">Sign Out</a>
        </div>
    </div>
    <div class="topnav">
        <a class="active">Grimpa Door Controller</a>
        <a href="/dashboard">Home</a>
        <a href="/pushSettings">Push Settings</a>
        <a href="/update">Update</a>
        <a href="/wifi">Network</a>
        <a class="pseudologo">Credentials</a>
        <a href="/control">Control</a>
        <a href="/">Sign Out</a>
    </div>
   <center> 
       <div class="container">
        <form id="credentialsForm" onsubmit="return readForm('credentialsForm');" action="/ChangeCredentials" method="GET">
                <label for="uname">Username</label><br>
                <input type="text" value="%PLACEHOLDER_LOGIN_UNAME%" name="uname"><br>
                <label for="psw">Password</label><br>
                <input type="password" style="margin-bottom: 0;" value="%PLACEHOLDER_LOGIN_PSW%" id="pswInp" name="psw"><br>
                <div class="showPwLeft"><input type="checkbox" id="check" onclick="showPass()"><label for="check">Show Password</label></div><br><br>
                <button class="submitBtn" type="submit">Submit Access Credentials</button>
        </form>
    </div>
</center>
</body>
<div class="footer">%PLACEHOLDER_SWVERSION%</div>
<script>
    function showPass() {
        var x = document.getElementById("pswInp");
        if (x.type === "password") {
            x.type = "text";
        } else {
            x.type = "password";
        }
    }

    function showDropdown() {
        document.getElementById("myDropdown").classList.toggle("show");
    }
    window.onclick = function (event) {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    window.ontouchend = function (event) {
        setTimeout(timedFunc, 500);
    }

    function timedFunc() {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    function readForm(formName = "settingsForm") {
        let formData = Array.from(document.querySelectorAll('#' + formName + ' input, select')).reduce((acc, input) => ({ ...acc, [input.name]: input.value }), {});
        let action = document.getElementById(formName).action;
        let params = '?';

        for (name in formData) params += name + '=' + formData[name] + '&';

        let urltoSend = action.concat(params).slice(0, -1);
        console.log(urltoSend);

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) alert(xhttp.responseText);
        };

        xhttp.open("GET", urltoSend, true);
        xhttp.send();

        return false;
    }
    function doGetRequest(endpoint) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(xhttp.responseText);
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }
</script>
)rawliteral";


/////////////////////////////////////////////////////////////////////// AP PUSH SETTINGS ///////////////////////////////////////////////////////////////////////
String AP_pushSettings_html=R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Qibixx PushSettings</title>
</head>

<body>
    <div class="dropdown">
        <button onclick="showDropdown()" class="dropbtn">Grimpa Door Controller &nbsp;&#9776;</button>
        <div id="myDropdown" class="dropdown-content">
            <a href="/dashboard">Home</a>
            <a class="pseudologo">Push Settings</a>
            <a href="/update">Update</a>
            <a href="/wifi">Network</a>
            <a href="/credentials">Credentials</a>
            <a href="/control">Control</a>
            <a href="/">Sign Out</a>
        </div>
    </div>
    <div class="topnav">
        <a class="active">Grimpa Door Controller</a>
        <a href="/dashboard">Home</a>
        <a class="pseudologo">Push Settings</a>
        <a href="/update">Update</a>
        <a href="/wifi">Network</a>
        <a href="/credentials">Credentials</a>
        <a href="/control">Control</a>
        <a href="/">Sign Out</a>
    </div>
    <form id="settingsForm" onsubmit="return readForm();" action="/ChangePushSettings" method="get">
        <div class="container">
            <center><div>
                <label for="pushProtocol">Push Protocol</label><br>
                <select class="selector" id="pushProto" name="pushProtocol" onChange="doReload();">
                    <option value="%PLACEHOLDER_PUSHPROTOCOL_ACTIVE%"> %PLACEHOLDER_PUSHPROTOCOL_ACTIVE% </option>
                    <option value="%PLACEHOLDER_PUSHPROTOCOL_INACTIVE%"> %PLACEHOLDER_PUSHPROTOCOL_INACTIVE% </option>
                </select> <br> 
                <label for="server">%SERVER_LABEL%</label><br>
                <input type="text" value="%PLACEHOLDER_SERVER%" name="server" %SERVER_LABEL_HIDDEN%><br>
                <label for="bearerToken" %PLACEHOLDER_BEARER_HIDDEN% >Authorization Header</label><br %PLACEHOLDER_BEARER_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_BEARER_TOKEN%" name="bearerToken" %PLACEHOLDER_BEARER_HIDDEN%><br %PLACEHOLDER_BEARER_HIDDEN%>
                <label for="mqttTopic" %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>MQTT Topic</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_MQTT_TOPIC%" name="mqttTopic" %PLACEHOLDER_MQTT_TOPIC_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <label for="lampTopic" %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>MQTT Signage Topic</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_MQTT_LAMPTOPIC%" name="lampTopic" %PLACEHOLDER_MQTT_TOPIC_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <label for="mqttPort" %PLACEHOLDER_MQTT_PORT_HIDDEN%>MQTT Port</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_MQTT_PORT%" name="mqttPort" %PLACEHOLDER_MQTT_PORT_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <label for="mqttQos" %PLACEHOLDER_MQTT_QOS_HIDDEN%>MQTT QoS</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_MQTT_QOS%" name="mqttQos" %PLACEHOLDER_MQTT_QOS_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <label for="mqttRetain" %PLACEHOLDER_MQTT_RETAIN_HIDDEN%>MQTT Retain</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <select class="selector" name="mqttRetain" %PLACEHOLDER_MQTT_RETAIN_HIDDEN% >
                    <option value="%PLACEHOLDER_MQTT_RETAIN_ACTIVE%"> %PLACEHOLDER_MQTT_RETAIN_ACTIVE% </option>
                    <option value="%PLACEHOLDER_MQTT_RETAIN_INACTIVE%"> %PLACEHOLDER_MQTT_RETAIN_INACTIVE% </option>
                </select> <br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%> 
                <label for="mqttUname" %PLACEHOLDER_MQTT_UNAME_HIDDEN%>MQTT Username</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_MQTT_UNAME%" name="mqttUname" %PLACEHOLDER_MQTT_UNAME_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <label for="mqttPsw" %PLACEHOLDER_MQTT_PSW_HIDDEN%>MQTT Password</label><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <input type="text" value="%PLACEHOLDER_MQTT_PSW%" name="mqttPsw" %PLACEHOLDER_MQTT_PSW_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%><br %PLACEHOLDER_MQTT_TOPIC_HIDDEN%>
                <button class="submitBtn" type="submit">Submit Changes</button>
            </div></center>
    </form>
</body>
<div class="footer">%PLACEHOLDER_SWVERSION%</div>
<script>
    function showDropdown() {
        document.getElementById("myDropdown").classList.toggle("show");
    }
    window.onclick = function(event) {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }
    window.ontouchend = function(event) {
        setTimeout(timedFunc, 500);
    }

    function timedFunc(){
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {

                    openDropdown.classList.remove('show');

                }
            }
        }
    }

    function doReload(){
        let elements = document.getElementById("settingsForm").elements;
        let obj ={};
        for(let i = 0 ; i < elements.length ; i++){
            let item = elements.item(i);
            obj[item.name] = item.value;
        }
        let xmlHttp = new XMLHttpRequest();
        xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            location.reload();
        }
        xmlHttp.open("GET", "/ChangePushSettings?pushProtocol="+obj.pushProtocol, true); // true for asynchronous 
        xmlHttp.send(null);
    }

    function readForm(formId="settingsForm") {
        let formData = Array.from(document.querySelectorAll( '#'+formId+' input, select')).reduce((acc, input) => ({ ...acc, [input.name]: input.value }), {});
        let action = document.getElementById(formId).action;
        let params = '?';
        for (name in formData) params += name + '=' + formData[name] + '&';
        let urltoSend = action.concat(params).slice(0,-1);
    
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) alert(xhttp.responseText);
        };

        xhttp.open("GET", urltoSend, true);
        xhttp.send();
        return false;
    }

</script>
)rawliteral";


/////////////////////////////////////////////////////////////// WiFi Client Endpoints2  //////////////////////////////////////////////////

const char  WC_home2[] PROGMEM=R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<style>
    body {
        margin: 0;
        font-family: Helvetica, Arial, sans-serif;
        background: #fafafa;
    }

    .topnav {
        overflow: hidden;
        background-color: #333;
        height: 4vh;
        margin-bottom: 2vh;
    }

    .topnav a {
        float: left;
        color: #f2f2f2;
        text-align: center;
        padding: 1vh;
        text-decoration: none;
        font-size: 1.8vh;
    }

    .topnav a:hover {
        background-color: #ddd;
        color: black;
    }

    .topnav a.active {
        background-color: #559BCC;
        color: white;
    }

    .pseudologo {
        background-color: #000000;
        color: white;
        font-weight: bold;
    }

    .container {
        margin: 0 auto;
        width: 100%;
    }

    .container .col {
        background: lightblue;
    }

    .container .col p {
        padding: .25rem .75rem;
    }


    .submitBtn {
        background-color: #1E90FF;
        border-radius: 5px;
        border: 0;
        display: inline-block;
        cursor: pointer;
        color: #ffffff;
        font-family: Arial;
        text-decoration: none;
        width: 30vh;
        height: 5vh;
    }

    .submitBtn:hover {
        background-color: #559BCC;
    }

    .submitBtn:active {
        position: relative;
        top: 1px;
    }

    .resetBtn {
        background-color: #FF8000;
        border-radius: 5px;
        border: 0;
        display: inline-block;
        cursor: pointer;
        color: #ffffff;
        font-family: Arial;
        text-decoration: none;
        width: 30vh;
        height: 5vh;
    }

    .resetBtn:hover {
        background-color: #FF7F50;
    }

    .resetBtn:active {
        position: relative;
        top: 1px;
    }

    .defaultBtn {
        background-color: #FF3333;
        border-radius: 5px;
        border: 0;
        display: inline-block;
        cursor: pointer;
        color: #ffffff;
        font-family: Arial;
        text-decoration: none;
        width: 30vh;
        height: 5vh;
    }

    .dafaultBtn:hover {
        background-color: #CD5C5C;
    }

    .dafaultBtn:active {
        position: relative;
        top: 1px;
    }

    input[type=text],
    input[type=password] {
        width: 25%;
        padding: 0.9vh;
        height: 0.8vh;
        margin-bottom: 1.5vh;
    }

    input[type=checkbox]+label {
        font-size: 1.5vh;
        margin-top: 0.1em;
    }

    .selector {
        width: 26.5%;
        height: 3.3vh;
        margin-bottom: 1.5vh;
    }

    .dropdown {
        display: none;
    }

    .footer {
        position: fixed;
        right: 0;
        bottom: 0;
        left: 0;
        padding: 0.5vh;
        font-size: 2vh;
        background-color: #333;
        text-align: center;
        color: white;
    }

    .showPwLeft {
        position: absolute;
        margin-left: 37%;
    }

    @media screen and (max-width: 640px) {

        .showPwLeft {
            position: absolute;
            margin-left: 22%;
        }

        .topnav {
            display: none !important;
        }

        .dropdown {
            position: relative;
            display: inline-block !important;
            width: 100%;
        }

        .dropbtn {
            background-color: #559BCC;
            color: white;
            margin-top: 0px;
            padding: 1.5vh;
            border: none;
            width: 100%;
            font-size: 4vh;
        }

        .dropdown-content {
            display: none;
            width: 100%;
            padding-top: 0px;
            position: absolute;
            background-color: #333;
            min-width: 50px;
            overflow: auto;
            z-index: 1;
            font-size: 2vh;
        }

        .dropdown-content a {
            color: white;
            padding: 1em;
            text-decoration: none;
            display: block;
        }

        .show {
            display: block;
        }

        .formUpdate {
            width: 70%;
            border-radius: 15px;
        }

        input[type=text],
        input[type=password] {
            width: 50%;
            padding: 0.9vh;
            height: 0.8vh;
        }

        input[type=checkbox]+label {
            font-size: 1.5vh;
            vertical-align: center;
            margin-top: 0.1em;
        }

        .selector {
            width: 55% !important;
            height: 3.3vh;
            margin-top: 0.4vh;
        }
    }

    @media only screen and (min-width:600px) {
        .container .col {
            float: left;
            width: 50%;
        }
    }

    @media only screen and (min-width:768px) {
        .container .col {
            width: 33.333%;
        }
    }

    @media only screen and (min-width:992px) {
        .container .col {
            width: 25%;
        }
    }

    .ediv{
        margin: 0;
        position: relative;
        top: 10%;
    }
    .traffic-light {
        display: block;
        background-color: #333;
        width: 150px;
        height: 250px;
        border-radius: 10px;
    }

    .light {
        margin-top: 5vh;
        appearance: none;
        left: 50%;
        width: 80px;
        height: 80px;
        border-radius: 100%;
        display: block;
    }

    .modal {
        display: none;
        position: fixed;
        z-index: 1;
        padding-top: 15vh;
        left: 0;
        top: 0;
        width: 100%;
        height: 100%;
        overflow: auto;
        background-color: rgb(0,0,0);
        background-color: rgba(0,0,0,0.4);
    }

    .modal-content {
        background-color: #fefefe;
        margin: auto;
        padding: 20px;
        border: 1px solid #888;
        width: 60%;
    }

    .close {
        color: #aaaaaa;
        float: right;
        position: relative;
        font-size: 28px;
        font-weight: bold;
        z-index: 1;
    }

    .close:hover,
    .close:focus {
        color: #000;
        text-decoration: none;
        cursor: pointer;
    }
    .inpModal{
        width: 8vh !important;
    }
    .modalBtn {
        background-color: #1E90FF;
        border-radius: 5px;
        border: 0;
        display: inline-block;
        cursor: pointer;
        color: #ffffff;
        font-family: Arial;
        text-decoration: none;
        width: 10vh;
        height: 3.5vh;
    }
</style>

<body onload="fetchMissingData();">
    <div class="dropdown">
        <button onclick="showDropdown()" class="dropbtn">Grimpa Door Controller</button>
        <div id="myDropdown" class="dropdown-content">
            <a class="pseudologo">Home</a>
            <a href="/">Sign Out</a>
        </div>
    </div>
    <div class="topnav">
        <a class="active">Grimpa Door Controller</a>
        <a class="pseudologo">Home</a>
        <a href="/">Sign Out</a>
    </div>
    <form id="dummyForm">
        <center>
            <div class="container">
                <label for="deviceName">Device Name</label><br>
                <input id="deviceName" type="text" value="" name="ignore" readonly><br>
                <label for="currCount">Current Count</label><br>
                <input id="currCount" type="text" value="" name="ignore" readonly><br>
                <label for="totalExits">Total Exits</label><br>
                <input id="totalExits" type="text" value="" name="ignore" readonly><br>
                <label for="totalEntries">Total Entries</label><br>
                <input id="totalEntries" type="text" value="" name="ignore" readonly><br>
        </div>
    </center>
    </form>
    <center><button id="modalBtn" onclick="openModal()" class="submitBtn">Edit Count</button></center>
    <div id="settingsModal" class="modal">
        <div class="modal-content">
           <center><span class="close">&times;</span> 
           <!--button class="close">Add</button-->
           <button class="modalBtn" onclick="setDiff(1)" id="add1">Add + 1</button>
           <button class="modalBtn" onclick="setDiff(-1)" id="del1" style="background-color: #FF3333;">Del - 1</button><br>
           <br>
           <input class="inpModal" id="valueToAdd" type="text" id="addThis" value="" ><button onclick="setCount(document.getElementById('valueToAdd').value)" class="modalBtn" style="margin-left: 1vh;">Set</button><br>
           <button class="modalBtn" onclick="setCount(0)" style="width: 22vh;">Reset Count</button>
        </center></div>
      </div>      
    <center>
        <div class="traffic-light">
          <div class="ediv"><div class="light" style="background-color: #FF0000;" type="radio" id="color1"></div>
          <div class="ediv"><div class="light" style="background-color: #00FF00;" type="radio" id="color2"></div>
        </div>
        </center>
    <div id="swVersion" class="footer"></div>
</body>


<script>

var modal = document.getElementById("settingsModal");
var btn = document.getElementById("modalBtn");
var span = document.getElementsByClassName("close")[0];

function openModal() {
  modal.style.display = "block";
}

span.onclick = function() {
  modal.style.display = "none";
}

window.onclick = function(event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
}

    let currentCount = 0;
    let maxPeople = 0;
    let isFirstFetch = 1;

    function fetchMissingData(){
        fetchSwVersion();
        fetchMaxPeople();
        fetchDevName()
        fetchTotalExits();
        fetchTotalEntries();
        fetchLampState();
        //fetchCurrCount();
        openSocket();      
    }
   
    function fetchSwVersion() {
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getSwVersion")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById('swVersion').innerHTML = this.responseText;
                console.log(this.responseText);
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

    function fetchLampState() {
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getLampState")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let currColor = this.responseText;
                console.log("Fetched Lamp Color: ", currColor );
                let redBall = document.getElementById("color1");
                let greenBall = document.getElementById("color2");
                if (currColor=='green'){
                    redBall.style.backgroundColor = '#000000';
                    greenBall.style.backgroundColor = '#00FF00';
                    console.log("Updating color to green");
                }else if (currColor=='red'){
                    redBall.style.backgroundColor = '#FF0000';
                    greenBall.style.backgroundColor = '#000000';
                    console.log("Updating color to red");
                }
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

    function fetchDevName() {
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getDevName")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById('deviceName').value = this.responseText;
                console.log("DevName:".concat(this.responseText));
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }


    function fetchCurrCount() {
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getCurrCount")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById('currCount').value = this.responseText.concat("/").concat(maxPeople);
                currentCount = this.responseText;
                console.log("Current Count: ",this.responseText);
                console.log("MaxCount: ".concat(maxPeople));
                let result = currentCount - maxPeople;
                console.log("result is ".concat(result));
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }


    function fetchMaxPeople() {
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getMaxPeople")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                maxPeople = this.responseText;
                console.log("Max People",this.responseText);
                if (maxPeople==0) setTimeout(fetchMaxPeople, 100);
                else fetchCurrCount();
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

    function fetchTotalExits(){
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getTotalExits")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let exits = this.responseText;
                console.log("Exits", exits);
                document.getElementById('totalExits').value = exits;
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

    function fetchTotalEntries(){
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/getTotalEntries")
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let entries = this.responseText;
                console.log("Entries", entries);
                document.getElementById('totalEntries').value = entries;
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

    function openSocket(){
        let wsEndpoint = "ws://".concat(window.location.host).concat("/telemetry")
        let socket = new WebSocket(wsEndpoint);

        socket.onopen = function(e) {
            console.log("Socket Open");
        };

        socket.onmessage = function(event) {
            console.log(`[SockRCV] ${event.data}`);
            let strArr = event.data.split(",");
            for (let i = 0; i < strArr.length; i++) {
                let splitted = strArr[i].split(":");
                let key = splitted[0];
                let val = splitted[1];
                if (key=="entries"){
                    let currEntries = document.getElementById('totalEntries').value;
                    if (currEntries!=val || isFirstFetch==1){
                         document.getElementById('totalEntries').value = val;
                    }
                    
                }else if(key=="exits"){
                    let currExits = document.getElementById('totalExits').value;
                    if (currExits!= val || isFirstFetch==1){
                        document.getElementById('totalExits').value = val;
                    }
                    
                } else if(key=="total"){
                    // if (currentCount==val && isFirstFetch==0){
                    //     return;
                    // }
                    currentCount = val;
                    document.getElementById('currCount').value = val.concat("/").concat(maxPeople);
                    let result = currentCount - maxPeople;
                    console.log("result is ".concat(result));
                } else if (key=="lamp"){
                    let redBall = document.getElementById("color1");
                    let greenBall = document.getElementById("color2");
                    if (val=='green'){
                        let currColor = greenBall.style.backgroundColor;
                        if (currColor!='#00FF00'){
                            greenBall.style.backgroundColor = '#00FF00';
                            redBall.style.backgroundColor = '#000000';
                            console.log("Updating color to green");
                        }

                    }else if (val=='red'){
                        let currColor = greenBall.style.backgroundColor;
                        if (currColor!='#000000'){
                            redBall.style.backgroundColor = '#FF0000';
                            greenBall.style.backgroundColor = '#000000';
                            console.log("Updating color to red");
                        }
                    }
                }
                // console.log ("KEY is ".concat(key));
            } 
            isFirstFetch = 0;
            // alert(`[message] Data received from server: ${event.data}`);
        };

        socket.onclose = function(event) {
            if (event.wasClean) {
                alert(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
            } else {
                alert('[close] Connection Lost');
            }
        };

        socket.onerror = function(error) {
            alert(`[error] ${error.message}`);
        };
    }


    function setCount(count){
        if (count < 0) {
            alert("Invalid Action!");
            return;
        } 
        console.log("Will set count to".concat(count))
        let xhttp = new XMLHttpRequest();
        let endpoint = "http://".concat(window.location.host).concat("/setCount?currCount=".concat(count));
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(this.responseText);
            }
        };
        xhttp.open("GET", endpoint, true);
        xhttp.send();
    }

    function setDiff(diff){
        let newValue = parseInt(diff) + parseInt(currentCount);
        console.log("SetDiff was called. newval:".concat(newValue))
        if (newValue < 0){
            alert("Invalid Action!")
            return;
        }else{
            setCount(newValue);
        }
    }
    
</script>
</html>
)rawliteral";

#endif