import * as inquirer from 'inquirer'
import * as shelljs from 'shelljs'
import chalk from 'chalk'

interface Answer {
  password: string
  remote: string
  vpnUser: string
  vpnPassword: string
}

export type VpnInfo = Answer

const expectScript = (answer: Answer) => {
  return `expect<<EOF 
log_user 1
set timeout 1000
spawn sudo -S openconnect -u ${answer.vpnUser} ${answer.remote}
expect {
  "sudo" {send "${answer.password}\r"; exp_continue}
  "Password:" {send "${answer.vpnPassword}\r"}
}
expect eof
EOF
`
}

function ask(): Promise<Answer> {
  const prompts: object[] = []
  prompts.push({
    type: 'password',
    message: '当前用户密码:',
    name: 'password',
    validate: function (val) {
      if (val) {
        return true
      }
      return "输入当前用户密码"
    }
  })
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
  })
  prompts.push({
    type: 'input',
    message: 'VPN用户名:',
    name: 'vpnUser',
    validate: function (val) {
      if (val) {
        return true
      }
      return "输入VPN用户名"
    }
  })
  prompts.push({
    type: 'password',
    message: 'VPN密码:',
    name: 'vpnPassword',
    validate: function (val) {
      if (val) {
        return true
      }
      return "输入VPN密码"
    }
  })
  return inquirer.prompt(prompts)
}

export function connect() {
  ask().then(answers => {
    const remote = answers.remote.split('-').map(val => val.trim())[0]
    answers.remote = remote
    fastConnect(answers);
  })
}

export function fastConnect(info: VpnInfo) {
  if (!info.password) {
    console.log(chalk.red('未输入当前用户密码'))
    return
  }
  if (!info.remote) {
    console.log(chalk.red('未输入代理地址'))
    return
  }
  if (!info.vpnUser) {
    console.log(chalk.red('未输入VPN用户'))
    return
  }
  if (!info.vpnPassword) {
    console.log(chalk.red('未输入VPN密码'))
    return
  }
  shelljs.exec(expectScript(info))
}
