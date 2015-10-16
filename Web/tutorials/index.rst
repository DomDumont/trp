.. _turorials-index:

=========
Tutorials
=========

	
	


Basic principle
================

T.R.P. use a script langage(AngelScript)
and search for its data in the gamedata directory (at the same level as the executable directory).
If you want to use another folder (or if you work on multiple projects), you can indicate your
gamedata folder in the file settings.xml ::

    <settings>
    ...
    <datafolder directory="mygamedata"/>
    ...
    </settings>

Learn the AngelScript syntax
============================

AngelScript is very close to the c++ langage but there are still some differences (such as references)
Basically, no new or delete.
Pointers are replaced by @
You can simulate new and delete by usgin a scope trick . Let's see this example : ::


	class A;

	A @refA; // This is a reference (= a pointer)

	// This is a new

	{
	A a;    // The object a is instantiated, it should be deleted at the end of the scope
			
	@refA = @a; // But in fact, it will stay allocated because there is still a reference on it	
	}

	// This is a delete

	@refA = NULL; // No more reference on object 'a' so we can finally call delete

For any additionnal informations , please go directly to the AngelScript website

`Here <http://www.angelcode.com/angelscript/sdk/docs/manual/doc_script.html>`_






Try the samples
===============

T.R.P. comes by default with a lot of samples.

Simply launch the application, you'll fall over them.

It is strongly advised to open these script files (extension .rsc = Rototo Script File) and study them.




.. toctree::
	:maxdepth: 1
	
	tuto1-hello
	tuto2-sprite


