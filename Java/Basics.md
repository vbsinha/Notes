Java compiler converts Java code to bytecode. This bytecode is designed to run on the virtual machine: JVM (Java Virtual Machine).
Bytecode is platform independent.

*JRE* is Java Runtime Environment. It is a package of everything necessary to run a compiled Java program, including the Java Virtual Machine (JVM), the Java Class Library, the `java` command, and other infrastructure. However, it cannot be used to create new programs. To run programs only JRE is enough.
*JDK* is Java Development Kit, the full-featured SDK for Java. It has everything the JRE has, but also the compiler (`javac`) and tools (like `javadoc` and `jdb`). It is capable of creating and compiling programs. Required for Java development.
[Diagram](https://docs.oracle.com/javase/8/docs/)