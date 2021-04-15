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
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
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


<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.


<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Your Name - [@JosephEMassey](https://twitter.com/JosephEMassey)

Project Link: [https://github.com/JosephEMassey/eVTOL_Simulation](https://github.com/JosephEMassey/eVTOL_Simulation)
