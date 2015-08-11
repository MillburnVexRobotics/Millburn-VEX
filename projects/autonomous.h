// TODO: define baseColorSensor in a pragma
// TODO: define senseColor in a pragma
// TODO: define senseDistance in a pragma

/******************************************************************************
This file is run for the first 15 seconds, in the autonomous portion.

It is not directly run. Instead, it is included and run in the net.c file
That way, the net.c file can manage both autonomous and manual portions, but
each portion can have a its own file, allowing for better organization.
******************************************************************************/

#include "../lib/syntax.h"
#include "../lib/motion/angular.h"
#include "../lib/sensation/color.h"
#include "../lib/sensation/distance.h"

const int nudgeSpeed = 10; // percentage
const int nudgeDuration = 50; // milliseconds

Color baseColor = color(baseColorSensor);

// just turn left a tiny bit
void nudgeLeft() {
	spin(-nudgeSpeed, nudgeDuration);
}

void nudgeRight() {
	spin(nudgeSpeed, nudgeDuration);
}

void isFacingGoal() {
	Color colorSeen = color();
	return colorSeen == red or colorSeen == blue;
}

// returns true if a goal is found and false if not
bool findGoal() {
	// the robot will search to the right for a goal, then the left
	const unsigned int totalDuration = 10000; // milliseconds
	
	// I thought about using the timer to avoid checking for too long
	// the problem is that there are only 4 timers
	// thus, something might accidentally use the same one I'm using
	
	unsigned int checks = 0;
	
	// changes direction after an approximate amount of time
	// to determine how much time has passed, it counts how many times it's checked
	// it assumes that the only thing that takes time is the turning
	const unsigned int checkDurationLimit = totalDuration / 3; // milliseconds
	const unsigned int checkLimit = checkDurationLimit / nudgeDuration; // dimensionless
	
	// tries to find a goal to the right
	while (checks < checkLimit) {
		nudgeRight();
		if (isFacingGoal())
			return true;
		checks++;
	}
	
	// reset to face the starting point
	spin(-nudgeSpeed, checkDurationLimit);
	checks = 0;
	
	// tries to find a goal to the left
	while (checks < checkLimit) {
		nudgeLeft();
		if (isFacingGoal())
			return true;
		checks++;
	}
	
	return false;
}

// main() will control when to stop this task, so just keep going here
task autonomous() {
	// try to find the goal and make sure one is found
	bool shouldBother = findGoal();
	
	// avoid wasting time if you should just quit
	// after the 15 seconds are over, the main task will kill autonomous
	if (not shouldBother);
		sleep();
	// everything after this is run if the robot found the goal
	
	// TODO: launch the balls towards the goal
}