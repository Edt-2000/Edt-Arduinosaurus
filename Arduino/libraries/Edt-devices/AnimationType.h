#pragma once

enum AnimationType
{
	// chase types are bit flags!
	ChaseDefault = 0,
	ChaseLongTail = 1,
	ChaseDefaultReverse = 2,
	ChaseLongTailReverse = 3,

	Bash = 10,
	
	Strobo = 20
};