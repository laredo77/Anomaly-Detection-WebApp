# Anomaly Detection Server WebApp
WebApp application for flight investigation.
 
### Description
The Anomaly detection server web app is a tool designed to investigate flights. With this app, flight investigators are able to examine the data of their flight and make sure that it was normal or to detect any anomalies in the flight that may have happened.
The data are presented by a graph of a line based linear-regression or a minimum encloser circle which are calculated from the features of the plane that correlate.
The investigator is provided with a list of the detected anomalies with their timesteps and the list of correlating features.

### Installation
To use the application, **it is required to run it on Linux environment**. Download the project files and open your favorite node.js IDE working environment - our recommendation is [WebStorm](https://www.jetbrains.com/webstorm/). Using the Linux terminal, install the following: 
>* Install [express](https://expressjs.com/)
>* run `sudo dpkg --configure -a`
>* run `sudo apt install nodejs`
>* run `node-gyp configure`
>* run `node-gyp build`
>* to run the program use `node ./Controller/server.js` or use 'edit configuration' on your IDE to server.js as main.

To display the result, the user must have 2 csv files that contain the flight data.
Then you are able to run the command `node ./Controller/server.js` in order to run the app.

### Usage
![WebApp](https://user-images.githubusercontent.com/60240620/119850225-05a7c980-bf16-11eb-819d-24faab9857c5.jpg)

Run the program and use your favorite browser. Connect to the URL: http://localhost:8080/
Using the dedicated button, upload the csv file with the fligth data to the server to have the algorithm learn the normal state.
Using the dedicated button, upload the csv file with the fligth data to the server that may contain anomalies in order to be investigated by the algorithm.
Choose an algorithm to detect anomalies from the following:
>* Linear Regression-Based Investigation Algorithm: This algorithm studies the data of a normal flight and reports correlating features. For each pair of correlated features, the algorithm creates a linear equation using a linear regression method which is calculated by all the points that make up the features. The algorithm calculates the distance of the furthest point from the regression line and defines this distance as a threshold distance. 
Then, the algorithm checks each pair of features in the investegated flight data for all the points that were created by the correlated features. If there is a point where the distance from the regression line is greater than the threshold distance, the algorithm reports this point as an anomaly.
>* Hybrid Investigation Algorithm: First, the algorithm learns the valid flight data. Pairs of features that have a higher correlation than 0.9 will be investigated by the linear regression algorithm. Unlike the first algorithm, pairs of features that do not strongly correlated which means their correlation is less then 0.9 but higher than 0.5 will be investigated by a [smallest circle algorithm](https://en.wikipedia.org/wiki/Smallest-circle_problem#Welzl's_algorithm) invented by [Emo Welzl](https://en.wikipedia.org/wiki/Emo_Welzl) to detect anomalies.
This algorithm learns all the points of a pair of features and produces a minimum circle that contains all the points together. 
The algorithm calculates the distance of the furthest point from the center point and defines this distance as a threshold distance (This is the radius because necessarily this point sits on the equation of the circle). Then, the algorithm checks each pair of features in the investegated flight data for all the points the correlated features create. If there is a point with greater distance from the center than the distance of the threshold - it means, it is outside of the mimimum circle and the algorithm reports this point as an anomaly.

Press Submit.

Now, a dedicated graph will open for the user showing the data and the anomalies.
The investigator can select the features that were received as correlating from the list of features. Each pair of features will display their own graph and their deviation points according to the chosen algorithm detected. As well as a list of deviations by times.

In addition, the user can upload new files whenever they wish and re-investegate without closing and restarting the app.

**Additional features**
>* Users can send HTTP Post request via any programming language. The request contains the algorithm they want to use and the contents of the csv files.
The users will recive a JSON object containing the anomalies.

You can also see our YouTube video at /!\ ADD YOUTUBE LINK /!\ in which we explain about the app.

### Dictionary
Folders of the project 
> Model  
> View  
> Controller

/!\ ADD URL PICTURE /!\

### Technical explanation
The web application was written in Node.js, JavaScript and HTML.
The project was written according to the MVC (Model-View-Controller) code architecture, which separates the functionality of the data presentation from the processing and calculation of the data.
The Controller link between them is to provide safety and separation of responsibilities.
The algorithms for detecting anomalies was written in c++ and it binds to the project.

### Credits
This project is a continuation of our first project - DeskApp for detecting anomalies [(View here)](https://github.com/ofirshtrosberg/WpfApp1).
The project was written by Maoz Kosover, Ofir Shtrosberg and Itamar Laredo, as part of a project in Advanced programming 2 course, Bar-Ilan University.
