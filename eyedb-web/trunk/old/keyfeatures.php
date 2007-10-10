<? $title = 'Key Features'; include( 'header.php'); ?>

<h1>EyeDB Key Features</h1>

<br>
The main key features of the EyeDB OODBMS are:
<h2>Standard OODBMS features</h2>
<ul>
<li>persistent typed data management</li>
<li>client/server model</li>
<li>transactional services</li>
<li>expressive object model</li>
<li>inheritance</li>
<li>integrity constraints</li>
<li>methods</li>
<li>triggers</li>
<li>query language</li>
<li>application programming interfaces</li>
</ul>

<h2>Language orientation</h2>
<ul>
<li>a definition language based on the ODMG Object Definition Language (ODL)</li>
<li>a query language based on the ODMG Object Query Language (OQL)</li>
<li>a C++ binding</li>
<li>a Java binding</li>
</ul>

<h2>Genericity and orthogonality of the object model</h2>
<ul>
<li>inspired by the SmallTalk, LOOPS, Java and ObjVlisp object models
(i.e. every class derives from the <code>object</code> class and can
be manipulated as an object)</li>
<li>type polymorphism</li>
<li>binary relationships</li>
<li>literal and object types</li>
<li>transient and persistent objects</li>
<li>method and trigger overloading</li>
<li>template-based collections such as set, bag and array</li>
<li>multi-dimensional and variable size dimensional arrays</li>
</ul>

<h2>Support for large databases</h2>
<ul>
<li>databases up to several Tb (tera-bytes)</li>
</ul>

<h2>Efficiency</h2>

<ul>
<li>database objects are directly mapped within
the virtual memory space</li>
<li>object memory copy are reduced to the minimum</li>
<li>caching policy is implemented</li>
</ul>

<h2>Scalability</h2>
<ul>
<li>programs are able to deal with hundred
of millions of objects without loss of performance</li>
</ul>

<? include( 'footer.php') ?>
