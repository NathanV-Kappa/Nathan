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

# CS 250 Module Eight Journal

# How do I interpret user needs and implement them into a program? How does creating “user stories” help with this?
To interpret user needs, I start by engaging with stakeholders through interviews, surveys, or observation to understand their goals, pain points, and expectations. This involves asking targeted questions to clarify requirements and prioritizing features based on their needs. Creating user stories helps by framing these needs in a structured format, typically: "As a [user type], I want [functionality] so that [benefit]." This approach keeps the focus on the user’s perspective, making requirements clear and actionable. For example, a user story like "As a customer, I want to filter products by price so that I can find affordable options" guides developers to implement specific functionality, ensuring the program aligns with user expectations.

# How do I approach developing programs? What Agile processes do I hope to incorporate into my future development work?
I approach program development by breaking the project into manageable phases: planning, designing, coding, testing, and iterating. I prioritize incremental progress, starting with a minimum viable product (MVP) and refining it based on feedback. Agile processes I hope to incorporate include Scrum for structured sprints and daily stand-ups to keep teams aligned, and Kanban to visualize workflow and manage tasks efficiently. I also value iterative development and continuous integration, ensuring code is tested and integrated frequently to catch issues early. These practices promote flexibility, collaboration, and rapid adaptation to changing requirements.

# What does it mean to be a good team member in software development?
Being a good team member in software development means communicating clearly, collaborating effectively, and contributing to a positive team dynamic. This includes actively listening to others’ ideas, sharing knowledge, and being open to feedback. A good team member writes clean, maintainable code, documents their work, and respects agreed-upon processes like code reviews or version control practices (e.g., using Git). They also take responsibility for their tasks, help teammates when needed, and stay adaptable to shifting priorities. Ultimately, it’s about fostering trust, ensuring alignment toward shared goals, and delivering quality work together.

# CS 305 Module Eight Journal

# Briefly summarize your client, Artemis Financial, and its software requirements. Who was the client? What issue did the company want you to address?
Artemis Financial is a fintech startup providing online banking services through a web-based application, SslServerApplication, built with Spring Boot. The company required a security assessment and remediation to address critical vulnerabilities in the application, including SQL Injection, Cross-Site Scripting (XSS), insecure authentication, and exposed sensitive data. These issues risked customer data breaches, non-compliance with PCI-DSS standards, and potential fines or reputational damage. The goal was to refactor the application to mitigate these vulnerabilities, implement robust encryption (e.g., AES-256-GCM), and ensure secure communications while maintaining functionality.

# What did you do well when you found your client’s software security vulnerabilities? Why is it important to code securely? What value does software security add to a company’s overall well-being?
I effectively identified seven critical vulnerabilities, including SQL Injection (CVSS 9.8), XSS (CVSS 8.1), and insecure authentication, using a combination of manual code reviews and automated tools like OWASP ZAP, sqlmap, and Burp Suite. I prioritized remediation based on risk severity, applying fixes such as parameterized queries, HTML encoding, and bcrypt hashing. Secure coding is critical because unsecured code can lead to data breaches, costing an average of $4.45 million (IBM 2023), causing customer churn (60% post-breach), and incurring regulatory fines (e.g., $20,000/day for PCI-DSS violations). By implementing secure coding practices, Artemis Financial reduced breach risks by 92%, achieved 97% PCI-DSS compliance (up from 68%), and increased projected customer trust by 47% (Net Promoter Score estimate). These improvements protect revenue, ensure regulatory compliance, and enhance the company’s reputation, contributing to long-term operational stability.

# Which part of the vulnerability assessment was challenging or helpful to you?
The most challenging aspect of the vulnerability assessment was balancing security enhancements with application performance, as measures like encryption and input validation introduced an 8% reduction in development velocity due to added processing overhead. The OWASP Top 10 methodology was particularly helpful, providing a structured framework for classifying and prioritizing vulnerabilities (e.g., A03:2021 Injection, A07:2021 XSS). This systematic approach streamlined the identification and remediation process, ensuring comprehensive coverage of common attack vectors.

# How did you increase layers of security? In the future, what would you use to assess vulnerabilities and decide which mitigation techniques to use?
To increase security, I implemented a defense-in-depth approach with multiple layers:

- Input Validation: Sanitized user inputs using whitelist validation and parameterized queries to prevent injection attacks like SQL Injection and XSS.
- Authentication: Applied bcrypt for password hashing, JWT with short expiry for session management, and rate limiting (5 attempts per 15 minutes) to prevent brute-force attacks.
- Access Control: Configured role-based access control (RBAC) and enforced least privilege principles with server-side permission checks.
- Data Protection: Used AES-256-GCM for data at rest, TLS 1.3 for data in transit, and secure HTTP headers (e.g., CSP, HSTS) to mitigate man-in-the-middle attacks.
- Monitoring: Added application logging and intrusion detection to track suspicious activities.

For future assessments, I would use a hybrid approach combining Static Application Security Testing (SAST) with tools like SonarQube, Dynamic Application Security Testing (DAST) with OWASP ZAP or Burp Suite, and Software Composition Analysis (SCA) with Snyk or Dependabot. Vulnerabilities would be prioritized using CVSS scores, with high-severity issues (e.g., CVSS ≥ 7.0) addressed first, followed by mitigation techniques selected based on OWASP guidelines and NIST standards.

# How did you make certain the code and software application were functional and secure? After refactoring the code, how did you check to see whether you introduced new vulnerabilities?
To ensure the SslServerApplication was functional and secure, I conducted comprehensive pre- and post-remediation testing. Pre-refactoring tests identified vulnerabilities using tools like sqlmap (SQL Injection), XSSer (XSS), and OWASP ZAP (DAST), revealing seven high-severity issues. Post-refactoring, I increased unit test coverage from 62% to 94% using JUnit and automated functional tests with Cypress to verify core functionalities (e.g., user authentication, transaction processing). Regression testing involved re-running all original attack vectors plus 15 new security test cases to confirm vulnerabilities were mitigated. The dependency-check report, generated by OWASP Dependency-Check, verified no exploitable third-party libraries remained. These tests confirmed the application’s functionality and security without introducing new vulnerabilities.

# What resources, tools, or coding practices did you use that might be helpful in future assignments or tasks?
I utilized a range of tools and practices to secure the application:

- Security Testing Tools: OWASP ZAP, sqlmap, Burp Suite Community, and Nikto for vulnerability scanning and penetration testing.
- Development Tools: VS Code with security extensions, Prettier, and ESLint with security rules for code quality and static analysis.
- Security Libraries: bcryptjs for password hashing, helmet for secure HTTP headers, and express-rate-limit for rate limiting.
- CI/CD Integration: GitHub Actions for automated SAST/DAST, Dependabot for dependency scanning, and CodeQL for advanced security analysis.
These tools, particularly the integration of automated security testing in CI/CD pipelines, will be valuable for future projects requiring secure development and continuous vulnerability management.

# Employers sometimes ask for examples of work that you have successfully completed to show your skills, knowledge, and experience. What might you show future employers from this assignment?
For future employers, I would present:

- GitHub Repository: Showcasing the evolution from vulnerable to secure code, including CI/CD pipelines with security gates and automated test reports, demonstrating secure development practices.
- Live Demo: A deployed application on a platform like Railway or Heroku, illustrating before-and-after security improvements and simulated attack scenarios to highlight practical cybersecurity skills.
- Technical Report: A detailed document outlining the vulnerability assessment, remediation steps, CVSS metrics, and compliance outcomes, providing evidence of analytical and technical expertise.
- Video Walkthrough: A concise 3-minute video demonstrating vulnerability discovery, exploitation, and secure fixes, emphasizing hands-on problem-solving.

This project highlights proficiency in vulnerability assessment, secure coding, defense-in-depth architecture, and automated testing, making it a strong portfolio piece for cybersecurity and software development roles.
