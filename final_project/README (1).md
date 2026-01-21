# FIFTYVILLE HEALTH INFORMATION SYSTEM
#### Video Demo: (https://youtu.be/N7SWvE5WUHM)
#### Description:

# What does it do?
 - This is a web application where hospital staff from a fictitious hospital can manage their specific unit's patients records. It uses Python (Flask), HTML/CSS/JS, Bootstrap and SQLite for the database.

# Why did I build it?
- For this is my background, I work as a technical nurse on a hospital in my city, and I use health
  systems daily, and there is where I got this inspiration to create this project, along with the knowledge I was gaining throughout the course.

### All the files it contains:

# app.py:
- This is the central part of this project, it's where the backend happens. The main Flask application, responsible for nearly every aspect of the backend logic, and it's what links the interface with the database. At the top we have the necessary libraries imported, including Flask itself, CS50's SQL library for easier usage and management of databases in Python, and I imported also Werkzeug's security functions for password hashing and verification. These imports, actually, highlights the project's objective, that is security and reliability; ensuring the data is protected.

- The file begins by configuring the Flask application, which, as spoke by David Malan, could be considered as boilerplate. Then I set up the session management. Connect to the SQLite database, in this case "hospital.db". This configuration allows the app to maintain user sessions from multiple requests. Also here I established the database connection, making it available to all the routes defined later in the file.

- Then there's a series of routes that correspond to different functionalities of the hospital management system. The root route (`/`) checks whether a user is logged in and, if so, it presents relevant, practical data on the homepage, such as the patients assigned to their unit, all users and all patients (Later inside the html we'll see that Jinja does the logic of filtering what each user role can have access to). This ensures that the homepage is personalized to the logged-in user's role and responsabilites, just as it happens in the real world, I tried to do it as straightforward as possible.

- The `/register` route handles the user creation, with some validation logic for the user's input, such as the role, the unit and password confirmation, as well as if the username is already taken, that ensures secure credential storage in the database. Whether any of these inputs are invalid, an error page will appear, with a message alerting the user on what happened wrong.

- Similar to the register page, the `/login` route and the `/logout` route manages authentication, ensuring that only those verified users can have access to confidential, sensitive information.

- Then we have the `/patients/<int:patient_id>` route, one of the most complex ones, which displays detailed patient information, and parse what should be displayed and what not. This route not only retrieves patient records but also allows users to submit notes about that specific patient, and their vital signs. The logic here includes validation checks to prevent invalid (or empty) submissions and type errors, as well as proper medial thresholds that trigger alerts when abnormal values are detected. For example, if a patient's oxygen saturation falls bellow 95%, the system automatically generates alerts and suggests possible diagnostic exams, only available for the doctors to see (jinja template logic), all that demonstrates that this file integrates medical logic into the application, as close to the real world as possible.

- Some functions avaible only for the admin role is handled by the `/admin` route. This route allows administrators to add and delete patients, manage users from the system, change patients and user's units, etc. Each action is validated to prevent errors, such as trying to move a patient to the same unit they were already assigned (Same for users). All these operations are somewhat "centralized", enforcing role-based access control, and ensuring that only authorized personnel can make structural changes inside the system.

- Finally the final route is `/users/<int:user_id>`, which provides a way to view some details about all the users registered on the website. One of these details are all the notes written by a specific user, linking their contributions to a specific patient record. This operation can promote transparency, allowing only the staff to track who added which notes and when.

- Overall, this app.py file is the heart of the project. It configures the application, manages user sessions, defines all the routes, validates the user input, it also enforces role-based permissions, and, as seen in the patient details route, it integrates medical logic. This file tie together the frontend templates, the database and the overall user experience into a cohesive hospital management system.

# hospital.db:
- This is the SQLite database that powers the backend of the management system. It serves as a storage layer, ensuring that all the user accounts, patient records, medical notes, and vital signs are securely saved and easily retrievable across all user sessions. Without this file, the application would be stateless, it wouldn't be able to preserve data between different users and their requests. The schema is composed of four tables: users, patients, vitals and notes. Each one is carefully designed to reflect real-world hospital workflows, while maintaining relational integrity.
> Users Table - Stores all registered accounts, with five specific columns:
- id;
- username;
- hash;
- role;
- unit;

- The inclusion of roles and units allows the application to enforce the role-based access control spoken before, and to have unit-specific data visibility, in order to make it as close to the real world as it can. Also the hashed password ensures that the user password is secured in the database with a criptographic transformation.

> Patients Table - Representing individuals admitted to the hospital, with five different columns:
- id;
- name;
- sex;
- unit;
- age;

- The unit column ties patients to specific hospital divisions, allowing staff to filter and view only the patients hospitalized on their assigned unit. This design mirrors real hospital operations, where staff are typically responsible for patients in their own department, not all the departments in the hospital.

> Vitals Table - Records physiological measurements of patients, included in the following columns:
- id;
- patient_id;
- systolic;
- diastolic;
- heart_rate;
- respiratory_rate;
- oxygen_saturation;
- temperature;
- recorded_at;

- Each entry is linked to a patient via the patient_id foreign key, making sure that the vitals are always associated with the correct patient.
- The recorded_at column automatically timestamps each vital signs entry, making chronological tracking of a patient's health status available.
- This design allows the alert system, which flags abnormal values and suggests diagnostic exams based on them.

> Notes Table - Stores written observations and comments made by hospital personnel. This table includes:
- id;
- patient_id;
- content;
- writer;
- date_time;

- Each note includes the patient_id it refers to, the content of the note, the writer (which includes their role and username), and a timestamp. This schema promotes collaboration from the registered staff, as they can see who wrote each note and when. It also provides a narrative layer to the patient care data, which complements and links to the numerical data previous seen in the vitals table.

- Together, these four tables form a hospital relational database. Foreign keys enforce relationships between patients, their vitals, notes, and prevents empty records. The schema can easily support future expansions, such as adding new roles, units, or medical record types.

- So hospital.db is where the project data management happens. It transforms the application from a simple frontend interface into a true hospital information system, with secure authentication and individual identification, structured patient care tracking, and collaborative note-taking. It is definitely a critical component of the project.

# requirements.txt:
- This is a simple but fundamental file in this project. It lists all the Python dependencies that must be installed for the web application to run correctly. With this file included, anyone who downloads the repository can set up the environment with a single command (pip install -r requirements.txt), and they can have access to the same libraries and versions as the web application developer.
- It contains the following dependencies:
> cs50
- It provides the SQL library used throughout the project, simplifying the database interations by allowing queries to be executed inside app.py, using python dictionaries, making the code easier to read.

> Flask
- The web framework that powers the application. It enables the definition of different web routes, rendering of templates, and handling of HTTP GET and POST requests.

> Flask-Session
- It extends the Flas session management capabilities. And its configured (inside app.py) to store session data on the server side. This ensures that the user authentication persists across multiple requests.

- Of note, the pytz, datetime, and requests libraries were initially considered for inclusion in requirements.txt, but were later removed. This is precisely because SQLite already handles timestamping automatically through the DEFAULT CURRENT_TIMESTAMP setting in the schema, and also because the web application does not make any external API calls.

- In conclusion, requirements.txt exists to configure the web application environment, with these essential tools: CS50's SQL library for database operations, Flask for web development, and Flask-Session for continuous authentication.

# flask_session:
- This file configures how sessions are handled in the hospital management system. With Flask-Session, this this file ensures the sessions are stored securely on the server side using the fylesystem. On app.py, SESSION_TYPE is set to "filesystem" and SESSION_PERMANENT = False. That means user login data is kept on the server and automatically expires when the browser closes (don't store it on cookies). It makes sure that the patient data is protected by keeping session information out of client-side cookies.

# /project/static/styles.css
- This file defines the visual identity of the hospital management system. It provides all the custom styling that complements Bootstrap's defaults, ensuring the application looks professional and consistent across pages. It focuses particularly on readability, usability and a modern "glass" aesthetic, that uses transparency and blur effects to create a cleaner, calmer interface.
> Global styles:
- Smooth scrolling (html {scroll-behavior: smooth; }), a white navbar, and a background image (`/project/static/hospital.jpg`), with a transparent overlay to soften the visuals.

> Typography:
- Some headings and the navbar brand are boldened for emphasis.

> Tables:
- Some custom colors for headers, hover effects, and some "glass" table variants that use transparency and blue for a more modern look.

> Buttons:
- .btn-custom, .btn-glass and .danger-btn-glass provide color schemes and hover states, which makes it interactive with the user and feel more responsive.

> Login/Register containers:
- Centered, card-like boxes with blur and shadow effects to highlight forms against the background.

> Forms:
- Rounded inputs, styled selects, and "glass" submit buttons that match the overall theme of the page.

> Patient/Dashboard headers:
- Decorative blocks with background blur, shadows and some colors to visually separate sections.

> Alerts:
- .warning-alert-glass, .danger-alert-glass, etc, provide contextual feedback with color-coded transparency effects.

> Textareas and cards:
- Also styles with blur and rounded corners to maintain consistency with the glass aesthetic.

# project/templates/layout.html
- This is the base template for the hospital management system, it provides the common structure and resources that all other pages inherit or "extends", it ensures consistency across applications.
> Head Section:
- Meta tags for encoding and responsiveness, links Bootstrap CSS/JS, Google Fonts, Font Awesome Icons, and the project's custom stylesheet (styles.css).
- It also defines the favicon (hospital.png) and the title block.

> Navigation Bar:
- Displays the hospital brand and if the user is logged in shows a logout button.

> Template blocks:
- Includes {% block title %} and {% block body %}, which child templates override to include page-specific content.

> JavaScript utilities:
- Adds a small script to detect online/offline system status and update text and element classes accordingly. Also another function to disable submit buttons after a form is submitted to prevent duplicate requests.

# project/templates/login.html

- This template renders the login form page. At top it extends the layout.html, inheriting the global structure, styles and navbar, and defines it's own content inside the block body section.
- The title block sets the page title to "Login Page", appearing in the browser tab.
- The login container is a styled card (.login-container), that centers the login form and provides headings that welcome users to the Fiftyville Health Information System.
- The form itself uses the POST method to send the user input to the `/login` route in app.py.
- Inside it contains two inputs, one for the username and one for the password, both styled with the Bootstrap's class .form-control.
- A submit button styled also with Bootstrap's .btn-custom class.
- At the end a smaller paragraph that provides a quick link to the registration page for new users who don't have an account yet.

# project/templates/register.html

- This template renders the user registration page of the hospital management system. Just like the previous page, it extends the layout.html template, and defines it's content inside the block body.
- A register container with a styled card (.register-container) that centers the form and provides a heading that invites users to create an account.
- Uses a POST method to send data to the `/register` route in app.py. Includes dropdowns for selecting role (doctor, nurse, admin) and unit (ICU, ER, Ward, admin)
- Three inputs, one for username, one for password and one to confirm the password, all marked as required.
- Also below it a login link that redirects the user back to the login page in case the users already have an account.

# project/templates/main_page.html

- The teplate that renders the homepage for logged-in users. It layout.html, and adapts it's content based on the user's role.
- The title block dynamically sets the page title to "<role> Homepage", based on the role the user is signed by.
- The body block displays the appropriate dashboard by including three different pages: doctor_dashboard.html, nurse_dashboard.html and admin_dashboard.html.
- The container styling uses Bootstrap's .container for consistent layout and spacing.

# project/templates/error.html

- This template basically shows users errors that may have occurred while using the web application, such as validation errors, unit changes between patients and users, password confirmation, and ensuring that usernames are not repeated.
- It is also inside a Bootstrap container, for a layout consistent with the rest of the website. A header mentions "Error" to the user, with an icon next to it.
- Right below the header, a message that uses Jinja syntax shows the specific error that the user encountered, so that they can make sure to change their input, depending on the case.
- Below the message is a quick link for the user to return to either the homepage or the login screen, and in this case, if the user is not logged in and clicks to go to the homepage, they will be redirected to the login screen.
- This template aims to be user-friendly, because it's one thing to simply prevent an action from happening, and another to inform the user so that they can change their response.

# project/templates/patients_table.html
- This template serves a patients table as a snippet to other dashboards, with a section title introducing "My patients" section, with a bed-pulse icon next to it.
- And right below it the patients table, where it shows the patients details, including their name, age, sex and unit. This page is also styled using Bootstrap classes (.table, .table-striped, .table-hover) and a custom "glass" effect for a modern look.
- Along with each row, there's a "View" button, linking to the patient's detailed page (`/patients/{{ patient.id }}`).
- Then there's also a conditional rendering, that if there's no patient assigned for that unit yet, it displays a simple styled message: "No patients assigned for this unit".

# project/templates/doctor_dashboard.html

- This template renders the doctor's dashboard within the hospital management system. It provides a clear overview of all the patients assigned to the logged-in doctor's unit.
- The header displays "Doctor Dashboard" with a Font Awesome doctor icon and the doctor's unit in uppercase.
- Then it's used Jinja syntax to include the patient's table related to that logged in doctor's unit, with `patients_table.html`

# project/templates/nurse_dashboard.html

- This template renders the nurse's dashboard in the hospital management system. It provides an overview of all the patients assigned to the nurse's unit.
- A header displaying "Nurse Dashboard" along with a correspondend Font Awesome icon next to it, and the unit name in uppercase.
- Similar to the doctor_dashboard, it uses Jinja to include the separated patients table.

# project/templates/admin_dashboard.html

- This template renders the administrator's dashboard. Unlike doctor an nurse dashboards, it provides a system-wide view and management tools.
- There's a users table, which lists all registered users with their ID, username, role and unit. And each row includes a "View" button, that points to `/users/{{ user.id }}` for detailed user management.
- Then also a patients table, that shows all the patients across units, including their ID, name, age, sex and unit. Each row comes along also with a "View" button linking to the patient's detailed page.
- An admit patient form, allowing admins to add new patients, and the fields include name, sex, and unit, and the form submits to `/admin`, using a hidden form_type value for operation identification.
- And lastly the system status, which displays online/offline status using serverDiv and serverStatus, updated via JS in layout.html.

# project/templates/user_detail.html

- This template displays detailed information about a specific user registered in the system.
- Inside there's a header containing a user icon, their username, role and unit.
- A notes section, which lists all notes registered by the user in a table (patient, content, date recorded)
- And if there isn't any notes registered by that user, it displays a message informing just that.
- There's also a change unit form, which allows admins to reassing the user's unit (ICU, ER, Ward, Admin), and also uses hidden fields to identify the form operation type (change_unit) and passes along the user ID to app.py.
- Lastly a delete user form, that provides an option to remove the user entirely. For security, there's a confirmation prompt to prevent accidental deletion. It also uses hidden fields to identify the form type.

# project/templates/patient_detail.html

- This template renders the detailed view of a specific patient in the system. It combines patient information, their clinical data, and some role-specific actions in it.
- As for the header, it shows the patient name and age, styled with badges, and a back button to retun to the homepage.
- There's an add vital signs form (available only for nurses and admins), for recording of new vital signs (systolic, diastolic, heart rate, respiratory rate, oxygen saturation and temperature). Each input box include validation ranges (min, max) to ensure realistic input. And also a disableSubmit() to prevent duplicated submissions.
- A latest vital signs table, which displays a table of the most recent vitals, also highlighting abnormal values in red and bold if the condition matches (for example, systolic > 140 or < 90). If there's no vitals registered yet, it shows a warning alert.
- A list with abnormal vital signs (available only for doctors and admins), which displays alerts for abnormal readings in a Bootstrap's danger-styled alert box. And if no abnormal vitals are found, it shows a success alert.
- A list with suggested exams (also available only for doctors and admins), which displays some recommended exams based specifically on the abnormal vital signs seen above. It uses different alert boxes to differentiate when there are suggested exams and when there are none.
- A vitals history table, displaying all recorded vital signs for that patient, with each physiological measurement styled with red and bold in case it's considered abnormal, for quick recognition. And if no vital signs are present, it shows a message in a warning alert.
- The notes table, listing all notes recorded about the patient, including content, author (role and username), and timestamp. Also if no notes were recorded yet, a warning alert is displayed.
- A form for creation of notes, with a hidden form_type field set to "note", directly linked to the `/admin` in app.py.
- A change patient unit form (available only for admins), allowing admins to reassign the patient to a different unit (ICU, ER, Ward), using hidden fields to identify the patient and the form type.
- Lastly an option to discharge the patient, also only for admins. Provides a form to remove the patient from the system, with a confirmation prompt to prevent accidental discharge. It uses hidden fields to identify the patient and the form type.

# Usage of AI during the project

- Throughout the developement of this project I leveraged AI tools to support and optimize my workflow.
- Mostly for styling ideas, AI helped me pick design choices and refine the CSS for a cleaner, more modern look.
- For Jinja tips, I discovered useful template functions, such as capitalize, title and include, which improved readability and reduced duplication in my templates.
- And some optimization guidance, where AI provede me some suggestions for simplifying template logic, reducing redundancy and structuring the application more efficiently.
- Overall, the AI assistance allowed me to focus on building the project while learning new features and techniques, improving the quality of the web application and my own personal development on code.

# Acknowlegdments
- A special thanks to `CS50` for the course framework and guidance, and to AI tools for assisting with styling ideas, Jinja tips and overall optimization.
