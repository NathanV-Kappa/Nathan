# CS 210 Module Eight Journal

# Summarize the project and what problem it was solving.
This project is to help set the current time and then adjust it by one hour, minute, or second individually. This program display the 12 and 24 hour clock, and a menu to help the user to adjust the time (and also how to exit the program as well). The goal of this project was to try to keep the main function as small as possible as well.

# What did you do particularly well?
I think I did particularly well with structuring my code and outputting the wanted 12 and 24 hour clock. Also, in minimizing the code in the main function by using other functions like void.

# Where could you enhance your code? How would these improvements make your code more efficient, secure, and so on?
Perhaps move the main menu print into a void function and then have the loop call the function whenever a new prompt is presented to user to change the time. This would help shorten the length of the main function.

# Which pieces of the code did you find most challenging to write, and how did you overcome this? What tools or resources are you adding to your support network?
The most challenging part of the code to write was realizing how to differentiate between 12 and 24 hour clock, which I solved this by using a conditional statement to subtract the 12 hour clock by 12 if it went above 12 hours. Then both follow when it hits the 24th hour of a day by resetting to 12:00 AM and 00:00:00 respectively.

# What skills from this project will be particularly transferable to other projects or course work?
The use of functions when using C++ will be useful when applying to other projects to print out and calculate whatever is the intended thing to be found.

# How did you make this program maintainable, readable, and adaptable?
By using comments and laying out in order of how both clocks work help made it very reable and maintainable. As without them it would take a bit of analyzing of what the code is trying to do and differentiate which is the 12 and 24 hour clock.
