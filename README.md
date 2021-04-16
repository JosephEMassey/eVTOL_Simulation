![Docker Build Status](https://img.shields.io/docker/cloud/build/josephemassey/evtol)
[![CircleCI Build Status](https://circleci.com/gh/circleci/circleci-docs.svg?style=shield)](https://app.circleci.com/pipelines/github/JosephEMassey)

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
      </ul>
    </li>
    <li><a href="#results">Results</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

There are five companies developing eVTOL aircrafts. The vehicles produced by each
manufacturer have different characteristics. Six distinct properties are laid out in the below table:

---------------------------------------------------------------------------
|                                 |   A   |   B   |   C   |   D   |   E   |
----------------------------------|-------|-------|-------|-------|-------|
| Cruise Speed (mph)              | 120   | 100   | 160   | 90    | 30    |
| Battery Capacity (kWh)          | 320   | 100   | 220   | 120   | 150   |
| Time to Charge (hrs)            | 0.60  | 0.20  | 0.80  | 0.62  | 0.30  |
| Energy use at Cruise (kWh/mile) | 1.6   | 1.5   | 2.2   | 0.8   | 5.8   |
| Passenger Count                 | 4     | 5     | 3     | 2     | 2     |
| Probability of fault/hr         | 0.25  | 0.10  | 0.05  | 0.22  | 0.61  |
---------------------------------------------------------------------------

The objective is to run a three minute simulation. For the sake of time, each second that
passes in real-time is equivalent to one minute of simulated time. So a three minute simulation
will allow three hours of simulated data to be produced. 20 total vehicles should be deployed, a
random number of each type of vehicle should be used (with the total between all five types
being 20).

There are only three chargers available for all 20 vehicles! A single charger can only be used by
one vehicle at a time. Assume the chargers can charge the batteries in the Time to Charge time
listed for each vehicle.

At the vehicle type level keep track of: average time in flight, average time charging, average
time waiting in line to charge, max number of faults, and total distance traveled by passengers
(if there are four passengers on a vehicle that cruises for a hour at 100 mph, total distance
traveled by passengers == 400 passenger-miles. If there are six instances of this vehicle in the
simulation 400 * 6 is the answer we are looking for).

Assume each vehicle starts the simulation with full battery, that it is airborne for the full use of
the battery, that it instantaneously reaches Cruise Speed, and that it instantaneously is in line
for the charger right when it runs out of battery.

At the end of the simulation represent the results of the simulation in whatever way you see
best.

### Built With

* [Visual Studio Code](https://code.visualstudio.com/)
* [Remote - Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
* [Docker](https://docker.com)



<!-- GETTING STARTED -->
## Getting Started

Clone repository and open in visual studio code.  Upon opening visual studio code will build docker container. 


<!-- RESULTS -->
## Results

Total Simulation Time: 180 mins
<table>
  <thead>
    <tr>
      <th><sub>Vehicle</sub></th>
      <th><sub>Num Vehicles</sub></th>
      <th><sub>Avg Flight Time (mins)</sub></th>
      <th><sub>Flight Time (%)</sub></th>
      <th><sub>Avg Charge Time (mins)</sub></th>
      <th><sub>Charge Time (%)</sub></th>
      <th><sub>Avg Qing Time (mins)</sub></th>
      <th><sub>Qing Time (%)</sub></th>
      <th><sub>Max Faults</sub></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><sub>A</sub></td>
      <td><sub>4</sub></td>
      <td><sub>106</sub></td>
      <td><sub>59.17</sub></td>
      <td><sub>21</sub></td>
      <td><sub>11.94</sub></td>
      <td><sub>51</sub></td>
      <td><sub>28.75</sub></td>
      <td><sub>3.00</sub></td>
    </tr>
    <tr>
      <td><sub>B</sub></td>
      <td><sub>4</sub></td>
      <td><sub>87</sub></td>
      <td><sub>48.75</sub></td>
      <td><sub>15</sub></td>
      <td><sub>8.33</sub></td>
      <td><sub>76</sub></td>
      <td><sub>42.50</sub></td>
      <td><sub>1.20</sub></td>
    </tr>
    <tr>
      <td><sub>C</sub></td>
      <td><sub>2</sub></td>
      <td><sub>74</sub></td>
      <td><sub>41.11</sub></td>
      <td><sub>48</sub></td>
      <td><sub>26.67</sub></td>
      <td><sub>58</sub></td>
      <td><sub>32.22</sub></td>
      <td><sub>0.30</sub></td>
    </tr>
    <tr>
      <td><sub>D</sub></td>
      <td><sub>3</sub></td>
      <td><sub>102</sub></td>
      <td><sub>57.04</sub></td>
      <td><sub>19</sub></td>
      <td><sub>10.93</sub></td>
      <td><sub>57</sub></td>
      <td><sub>31.67</sub></td>
      <td><sub>1.98</sub></td>
    </tr>
    <tr>
      <td><sub>E</sub></td>
      <td><sub>7</sub></td>
      <td><sub>100</sub></td>
      <td><sub>55.87</sub></td>
      <td><sub>18</sub></td>
      <td><sub>10.00</sub></td>
      <td><sub>60</sub></td>
      <td><sub>33.81</sub></td>
      <td><sub>12.81</sub></td>
    </tr>
  </tbody>
</table>


<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Your Name - [@JosephEMassey](https://twitter.com/JosephEMassey)

Project Link: [https://github.com/JosephEMassey/eVTOL_Simulation](https://github.com/JosephEMassey/eVTOL_Simulation)
