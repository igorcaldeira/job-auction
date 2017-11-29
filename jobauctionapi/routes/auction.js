var express = require("express");
var router = express.Router();
const readline = require("readline");
const fs = require("fs");

router.post('/', function(req, res, next) {

    //
    // get and set data from req
    //

    data = req.body;
    let employeeList = [];
    
    for (var size = 0; size < data.employeeListSize; size++)
      employeeList.push(data['employeeList['+size+']']);

    console.log(employeeList);

    //
    // create empolyee input files
    //

    fs.writeFileSync("funcionarios.txt", "", function() {
      console.log("done");
    });

    var logger = fs.createWriteStream("funcionarios.txt", {
      flags: "a" // 'a' means appending (old data will be preserved)
    });

    for (var size = 0; size < data.employeeListSize; size++) {
      logger.write(employeeList[size].id + "\r\n");
      logger.write(employeeList[size].name + "\r\n");
      logger.write(employeeList[size].adress + "\r\n");
      logger.write(employeeList[size].cpf + "\r\n");
      logger.write(employeeList[size].email + "\r\n");
      logger.write(employeeList[size].phoneOne + "\r\n");
      logger.write(employeeList[size].phoneTwo + "\r\n");
      logger.write(employeeList[size].setOne + "  \r\n");
      logger.write(employeeList[size].setTwo + "  \r\n");
      logger.write(employeeList[size].numberOne + "\r\n");
      logger.write(employeeList[size].letterOne + "\r\n");
      logger.write(employeeList[size].numberTwo + "\r\n");
      logger.write(employeeList[size].numberThree + "\r\n");
      logger.write(employeeList[size].status + " \r\n");
      logger.write("\r\n");
    };

    logger.end();

    //
    // call auction child process
    //

    var exec = require("child_process").execFile;

    var runMainCpp = function() {
      exec("main", function(err, data) {

        let outputData = {};

        const rlTasks = readline.createInterface({
          input: fs.createReadStream("funcionarios_out.txt")
        });

        const rlTeam = readline.createInterface({
          input: fs.createReadStream("projetos_out.txt")
        });

        const rlTable = readline.createInterface({
          input: fs.createReadStream("relatorio.txt")
        });

        //
        // tasks
        //

        var p1 = new Promise((resolve, reject) => {

          var rlTasksResult = "";

          rlTasks.on("line", function(line) {
            console.log("Line from file:", line);
            rlTasksResult += line + "/br";
          });

          rlTasks.on('error', function (error) {
            reject(error)
          });

          rlTasks.on('close', function () {
            let task = rlTasksResult.split("/br");
            let poneData = [];

            for (let i = 0; i < task.length; i += 15 ){
              poneData.push({
                0:  task[i + 0],
                1:  task[i + 1],
                2:  task[i + 2],
                3:  task[i + 3],
                4:  task[i + 4],
                5:  task[i + 5],
                6:  task[i + 6],
                7:  task[i + 7],
                8:  task[i + 8],
                9:  task[i + 9],
                10: task[i + 10],
                11: task[i + 11],
                12: task[i + 12],
                13: task[i + 13],
              });
            }

            resolve(poneData);
          });
        });

        //
        // team
        //

        var p2 = new Promise((resolve, reject) => {

          var rlTeamResult = "";

          rlTeam.on("line", function(line) {
            console.log("Line from file:", line);
            rlTeamResult += line + "/br";
          });

          rlTeam.on('error', function (error) {
            reject(error)
          });

          rlTeam.on('close', function () {
            let team = rlTeamResult.split("/br");
            let ptwoData = [];

            for (let i = 0; i < team.length; i += 15) {
              ptwoData.push({
                0: team[i + 0],
                1: team[i + 1],
                2: team[i + 2],
                3: team[i + 3],
                4: team[i + 4],
                5: team[i + 5],
                6: team[i + 6],
                7: team[i + 7],
                8: team[i + 8],
                9: team[i + 9],
                10: team[i + 10],
                11: team[i + 11],
                12: team[i + 12],
                13: team[i + 13],
              });
            }

            resolve(ptwoData);
          });
        });

        //
        // table
        //

        var p3 = new Promise((resolve, reject) => {

          var rlTableResult = "";

          rlTable.on("line", function(line) {
            console.log("Line from file:", line);
            rlTableResult += line + "/br";
          });

          rlTable.on('error', function (error) {
            reject(error)
          });

          rlTable.on('close', function () {
            
            resolve(rlTableResult.split("/br"));
          });
        });

        Promise.all([p1, p2, p3])
          .then(values => {

            let outputData = {
              tasks: values[0],
              team: values[1],
              table: values[2]
            }

            res.send(outputData);
          })
          .catch(err => {
            console.log(err);
          }) 

      });
    };

    runMainCpp();
});

module.exports = router;