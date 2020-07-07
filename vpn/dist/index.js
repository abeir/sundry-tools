"use strict";
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (Object.hasOwnProperty.call(mod, k)) result[k] = mod[k];
    result["default"] = mod;
    return result;
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
var inquirer = __importStar(require("inquirer"));
var shelljs = __importStar(require("shelljs"));
var chalk_1 = __importDefault(require("chalk"));
var expectScript = function (answer) {
    return "expect<<EOF \nlog_user 1\nset timeout 1000\nspawn sudo -S openconnect -u " + answer.vpnUser + " " + answer.remote + "\nexpect {\n  \"sudo\" {send \"" + answer.password + "\r\"; exp_continue}\n  \"Password:\" {send \"" + answer.vpnPassword + "\r\"}\n}\nexpect eof\nEOF\n";
};
function ask() {
    var prompts = [];
    prompts.push({
        type: 'password',
        message: '当前用户密码:',
        name: 'password',
        validate: function (val) {
            if (val) {
                return true;
            }
            return "输入当前用户密码";
        }
    });
    prompts.push({
        type: 'list',
        name: 'remote',
        message: '选择代理:',
        choices: [
            'a01.any00.com:1443 - Japan1',
            'a02.any00.com:1443 - Japan2',
            'a03.any00.com:1443 - Japan3',
            'a04.any00.com:1443 - Japan4',
            'a05.any00.com:1443 - HK,China',
            'a06.any00.com:1443 - Singapore, SG',
            'a07.any00.com:1443 - Fremont, CA, USA',
            'a08.any00.com:1443 - Singapore, SG',
            'a09.any00.com:1443 - US',
            'a10.any00.com:1443 - RU'
        ]
    });
    prompts.push({
        type: 'input',
        message: 'VPN用户名:',
        name: 'vpnUser',
        validate: function (val) {
            if (val) {
                return true;
            }
            return "输入VPN用户名";
        }
    });
    prompts.push({
        type: 'password',
        message: 'VPN密码:',
        name: 'vpnPassword',
        validate: function (val) {
            if (val) {
                return true;
            }
            return "输入VPN密码";
        }
    });
    return inquirer.prompt(prompts);
}
function connect() {
    ask().then(function (answers) {
        var remote = answers.remote.split('-').map(function (val) { return val.trim(); })[0];
        answers.remote = remote;
        fastConnect(answers);
    });
}
exports.connect = connect;
function fastConnect(info) {
    if (!info.password) {
        console.log(chalk_1.default.red('未输入当前用户密码'));
        return;
    }
    if (!info.remote) {
        console.log(chalk_1.default.red('未输入代理地址'));
        return;
    }
    if (!info.vpnUser) {
        console.log(chalk_1.default.red('未输入VPN用户'));
        return;
    }
    if (!info.vpnPassword) {
        console.log(chalk_1.default.red('未输入VPN密码'));
        return;
    }
    shelljs.exec(expectScript(info));
}
exports.fastConnect = fastConnect;
