# CS 230 Module Eight Journal

# Briefly summarize The Gaming Room client and their software requirements. Who was the client? What type of software did they want you to design?
The client, The Gaming Room, tasked a software engineer with designing a web-based game modeled after the 1980s TV game show "Draw It or Lose It." The software needed to support multiple teams and multiplayer functionality, ensure no empty teams during matchmaking, provide a unique and dynamic game experience with distinct team names, and prevent duplicate game instances in memory. These requirements were achieved using a singleton pattern for unique object instances and an iterator pattern to avoid naming conflicts.

# What did you do particularly well in developing this documentation?
I effectively incorporated object-oriented programming techniques, such as inheritance, abstraction, encapsulation, and polymorphism, into the design explanation, clearly illustrating their use through the Entity class and its subclasses (Game, Team, Player). Additionally, I provided comprehensive recommendations for selecting Linux as the operating platform, highlighting its scalability and security advantages for a multiplayer environment. The document also thoroughly addressed design constraints, including web compatibility, uniqueness, and scalability, supported by credible references for architectural choices.

# What about the process of working through a design document did you find helpful when developing the code?
Creating the design document proved valuable by enabling early mapping of the UML diagram and class relationships, which guided the implementation of the GameService class using a singleton pattern to ensure a single instance in memory. The detailed process of creating games, teams, and players through methods like addGame(), addTeam(), and addPlayer(), each utilizing the iterator pattern to prevent duplicates, simplified translating requirements into code without conflicts. This structured approach also emphasized the Entity base class for shared attributes, reducing null objects and promoting overloaded constructors, which streamlined coding and minimized errors.

# If you could choose one part of your work on these documents to revise, what would you pick? How would you improve it?
If I could revise one part of the documentation, I would focus on the section detailing the design constraints, particularly the discussion on balancing client-side and server-side processing. The current explanation briefly mentions performance concerns related to users' hardware and internet connectivity but lacks depth in addressing potential solutions. To improve it, I would expand this section by including specific strategies, such as implementing client-side caching for frequently accessed game assets or using WebSocket optimizations to reduce server load. Additionally, I would provide a clearer breakdown of how to profile and test performance across diverse devices and network conditions, ensuring the game remains responsive for all users. This would make the documentation more actionable for developers implementing the design.

# How did you interpret the user’s needs and implement them into your software design? Why is it so important to consider the user’s needs when designing?
I interpreted the user’s needs for multiplayer support, unique team experiences, and efficient memory usage by designing a system with a singleton pattern for the GameService to manage unique instances and an iterator pattern to prevent duplicate names. Cross-platform compatibility was ensured for web-based access across devices, supported by recommendations for scalable Linux servers with robust security. Considering user needs is vital to create intuitive, high-performing, and reliable software, preventing issues like performance delays or security risks that could frustrate players and lead to disengagement, aligning the design with usability and business objectives.

# How did you approach designing software? What techniques or strategies would you use in the future to analyze and design a similar software application?
My approach began with a main driver class and UML diagram to define relationships, followed by implementing design patterns like singleton for the GameService to ensure unique instances and iterator to prevent naming conflicts. I applied object-oriented programming principles (inheritance, abstraction, encapsulation, polymorphism) through the Entity class hierarchy and evaluated constraints like scalability and security. For future similar applications, I would continue using UML for visual planning, conduct detailed requirement analysis with stakeholder input, prototype key features early, and adopt agile iterations to test cross-platform compatibility. I would also expand the use of microservices and containerization, such as Docker on Linux, to enhance distributed system performance.


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
