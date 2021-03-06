<?xml version='1.0' encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<!-- The TOC links in the titles, and in blue. -->
<xsl:param name="latex.hyperparam">colorlinks,linkcolor=blue,pdfstartview=FitH</xsl:param>

<!-- Put the dblatex logo -->
<xsl:param name="doc.publisher.show">1</xsl:param>

<!-- List the examples and equations too -->
<xsl:param name="doc.lot.show">figure,table,example</xsl:param>

<xsl:param name="chunker.output.encoding">UTF-8</xsl:param>
<xsl:param name="textdata.default.encoding">UTF-8</xsl:param>

</xsl:stylesheet>
