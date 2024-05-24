# SAFT Utilitário
#### Video Demo: https://youtu.be/ompRqVyzmC8
### What's a SAFT anyways?
The Audit Standard File for Tax Purposes (SAF-T) is a standardized file (in XML format) that allows easy export, at any time, of a predefined set of accounting and billing records, in a readable format, regardless of the program used. The SAFT has a clear and objective intention: to define a standard language for reporting tax information in order to facilitate inspection and prevent tax fraud/evasion and prevent side-economy activities. This file is an international standard defined by the OECD, although each country can be controlled as needed.

### **Some context...**
In Portugal, companies of all kinds have to send this SAFT file to the state every month. So far so good! However, if the file is over 40MB and is uploaded via the normal method of using the fiscal authority website, an error will be raised because of its size.
The only workaround to this is by downloading a .jar file (Java console program), opening a Command Line (cmd.exe) and writing a long command that includes the path to this file and multiple arguments like Fiscal Number, password, month and year, SAFT file path, among others. As you may imagine, to the regular non-tech-savvy person, opening the Command Line alone might be a challenge, let alone compose a long command.

### **Why I made it**
ERP companies and software houses have obviously caught on to this and have tacked on a solution to their big products. However, most people won't subscribe to a big ERP in order to send a single file every month.
As such, I decided to create a small app that provides a simple and user-friendly way of uploading the SAFT file. As a bonus it can also be used for sub 40MB files, meaning it can be used to upload any SAFT in succession, something that professional accountants will certainly appreciate.

### **But how?**
The app was written using the .NET MAUI framework. Developed by Microsoft, it allows you to create multiplatform apps that can be run on both mobile and desktop. Although the program can only realistically be run on Desktop (.jar file must be run on console with commands), I went for MAUI because I wanted to learn something recent and different from what I usually work with. A second reason was because MAUI is part of .NET which I am already familiar with, having programmed in C# for work.

### **Some details**
MAUI uses a modified version of XML called XAML, to create the front-end of the app. This was fun to learn and it's pretty intuitive, albeit easy to lose track of things (but it's probably because it's just different from anything I've done before). For the code-behind, I used C# without any external libraries except for the ones MAUI offers.
I ran into some issues regarding the window sizing when transfering from one screen to another, but I've seen this happen in larger scale MAUI apps as well. You can always deal in absolute sizes but it looked worse that way.

### **How does it work?!**
It's pretty simple actually. The app is just a GUI for the actual .jar program. You still need to provide the same data that would serve as arguments on the command line, but with Textbox/Entry, DatePicker and FilePicker controls.
<p>
You also have access to both submission methods, Validate and Submit, in the form of two buttons. Validate sends the chosen SAFT files to the fiscal authority servers and replies if it's valid or not. An invalid file might have bad dates on invoices, or suspicious values. Submit on the other hand, actually submits the file to their servers; some validations are still run, but fiscal authority always suggests running a Validation before Submitting.
</p>
<p>
The app checks the basic information for input errors as well. It has an algorithm that checks if the NIF (Portuguese Fiscal Number) is valid or not; this is done in a way similar to what I had to write for the Credit problem. <br>
It also has two pickers for both Year and Month. Since you cannot create a SAFT file from a month that hasn't passed yet, the Year picker checks if you selected the current year; if you did, it will change the Month picker to only have months up to the current one. Any other year, you get months 1 to 12. <br>
I did my best to prevent null fields as well as presenting clean errors to the user.
<br><br><br>

# THIS WAS CS50. THANK YOU FOR THE RIDE!