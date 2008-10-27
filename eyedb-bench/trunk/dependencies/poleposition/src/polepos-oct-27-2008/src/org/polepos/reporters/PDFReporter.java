/* 
This file is part of the PolePosition database benchmark
http://www.polepos.org

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA  02111-1307, USA. */

package org.polepos.reporters;

import java.awt.*;
import java.awt.Font;
import java.io.*;
import java.util.*;
import java.util.List;

import org.jfree.chart.*;
import org.polepos.framework.*;

import com.lowagie.text.*;
import com.lowagie.text.pdf.*;


public class PDFReporter extends GraphReporter {
	private Circuit _circuit;
	private Document _document;
	private PdfWriter _writer;
	
	private LinkedList _pdfData = new LinkedList();
	private static final com.lowagie.text.Font h1Font = FontFactory.getFont(FontFactory.HELVETICA,15,Font.BOLD);
    private static final com.lowagie.text.Font h2Font = FontFactory.getFont(FontFactory.HELVETICA,12,Font.BOLD);
    private static final com.lowagie.text.Font bigFont = FontFactory.getFont(FontFactory.HELVETICA,10,Font.BOLD);
    private static final com.lowagie.text.Font smallFont = FontFactory.getFont(FontFactory.HELVETICA,9,Font.PLAIN);
    
    protected void report(Graph graph) {
    	if(_document == null) {
    		setupDocument(path());
    	}
        Circuit circuit = graph.circuit();
        if(! circuit.equals(_circuit)){
            _circuit = circuit;
        }
        
        JFreeChart timeChart = createTimeChart(graph);
        timeChart.setBackgroundPaint(null);
        try {
			renderTimeTable(graph);
			_pdfData.add(renderChart(timeChart));
			_pdfData.add(new NewPageLabel());
		} catch (Exception e) {
			e.printStackTrace();
		}
		// for overview
		createMemoryChart(graph);
    }

    private void renderFirstPage(List<TeamCar> cars) throws DocumentException{        
        Paragraph para=new Paragraph();
        para.add(new Chunk("PolePosition\n",h1Font));
        para.add(new Chunk("the open source database benchmark\n",smallFont));
        para.add(linked(new Chunk(ReporterConstants.WEBSITE + "\n\n\n", smallFont), ReporterConstants.WEBSITE));
        para.add(new Chunk("Participating teams\n\n",h2Font));
        
        int i = 0;
        _pdfData.add(i++, para);
        
        List <Object> printed = new ArrayList <Object>();
        
        for(TeamCar teamCar : cars){
            Team team = teamCar.getTeam();
            String webSite = team.website();
            if(webSite != null){
                if(! printed.contains(team)){
                    printed.add(team);
                    _pdfData.add(i++, renderTeam(team.name(), team.description(), webSite));
                }
            }else{
                Car car = teamCar.getCar();
                webSite = car.website();
                if(webSite != null){
                    if(! printed.contains(car)){
                        printed.add(car);
                        _pdfData.add(i++, renderTeam(car.name(), car.description(), webSite));
                    }
                }
            }
        }
        _pdfData.add(i++, new NewPageLabel());
    }
    
    private Element renderTeam(String name, String description, String website) throws DocumentException{
        Paragraph para=new Paragraph();
        para.add(linked(new Chunk(name + "\n",bigFont), website));
        if(description != null){
            para.add(linked(new Chunk(description + "\n",smallFont), website));
        }
        if(website != null){
            para.add(linked(new Chunk(website + "\n", smallFont), website));
        }
        para.add(new Chunk("\n",smallFont));
        return para;
    }
    
    private Element linked(Chunk chunk, String link){
        if(link == null){
            return chunk;
        }
        Anchor anchor = new Anchor(chunk);
        anchor.setReference(link);
        return anchor;
    }
    
    

	private void setupDocument(String path) {
		String fileName = path + "/" + "PolePosition.pdf";
		File file = new File(fileName);
		file.delete();
		_document = new Document();
		try {
			_writer = PdfWriter.getInstance(_document, new FileOutputStream(file));
			_document.open();
		} catch (Exception exc) {
			exc.printStackTrace();
			_document=null;
		}
	}

    @Override
	public void endSeason() {
        super.endSeason();
        if(_document != null){
            _document.close();
        }
        if(_writer != null){
            _writer.close();
        }
	}

	private void renderTimeTable(Graph graph) throws DocumentException {
		String unitsLegend = "t [time in ms]";
		renderTable(ReporterConstants.TIME, graph, unitsLegend);
	}
	
	private void renderMemoryTable(Graph graph) throws DocumentException {
		String unitsLegend = "m [memory in bytes]";
		renderTable(ReporterConstants.MEMORY, graph, unitsLegend);
	}

	private void renderTable(int type, Graph graph, String unitsLegend) throws BadElementException, DocumentException {
		Paragraph para=new Paragraph();
        Circuit circuit = graph.circuit();
        Lap lap = graph.lap();
        
        para.add(new Chunk("Circuit: " + circuit.name()+ "\n",bigFont));
        para.add(new Chunk(circuit.description() + "\n",smallFont));
        para.add(new Chunk("Lap: " + lap.name()+ "\n\n",bigFont));
        
		List<TeamCar> teamCars=graph.teamCars();
		List<TurnSetup> setups=graph.setups();
		Table table = setupTable(graph);
		int idx=1;
        addTableCell(table, 0, 0,unitsLegend , null,false,true);
		for(TurnSetup setup : setups) {
            StringBuffer header = new StringBuffer();
            boolean first = true;
            for(SetupProperty sp : setup.properties()){
                if(! first){
                    header.append("\n");
                }
                String name = sp.name();
                if(! name.equals("commitinterval")){
                    header.append(name);
                    header.append(":");
                    header.append(sp.value());
                    first = false;
                }
            }
			addTableCell(table, idx, 0, header.toString(),null, true,true);
			idx++;
		}
		table.endHeaders();
		int vidx=1;
		for(TeamCar teamCar : teamCars) {
			addTableCell(table,0,vidx,teamCar.toString(),teamCar.website(),true,false);
			int hidx=1;
			for(TurnSetup setup : setups) {
				String text = reportText(type, graph, teamCar, setup);
				addTableCell(table,hidx,vidx,text, null,false,false);
				hidx++;
			}
			vidx++;
		}
		para.add(table);
        para.add(new Chunk("\n",bigFont));
        _pdfData.add(para);
	}

	private String reportText(int type, Graph graph, TeamCar teamCar, TurnSetup setup) {
		String text = null;
		switch (type) {
		case ReporterConstants.TIME:
			text = String.valueOf(graph.timeFor(teamCar, setup));
			break;
		case ReporterConstants.MEMORY:
			text = String.valueOf(graph.memoryFor(teamCar, setup));
		}
		return text;
	}


	private void addTableCell(Table table, int hidx, int vidx, String text, String link, boolean bold,boolean header) throws BadElementException {
        Chunk chunk = new Chunk(text,FontFactory.getFont(FontFactory.HELVETICA,9,(bold ? Font.BOLD : Font.PLAIN)));
        chunk.setTextRise(3);
		Cell cell=new Cell(linked(chunk, link));
		cell.setHeader(header);
        if(! header){
            cell.setNoWrap(true);
        }
        cell.setVerticalAlignment(Element.ALIGN_CENTER);
		table.addCell(cell,new Point(vidx,hidx));
	}
	
	private Element renderChart(JFreeChart chart) throws DocumentException, BadElementException {
		return renderChart(chart, 500, 300);
	}
	
	private Element renderChart(JFreeChart chart, int width, int height) throws DocumentException, BadElementException {
        PdfContentByte cb = _writer.getDirectContent();
		PdfTemplate tp = cb.createTemplate(width, height);
		Graphics2D graphics = tp.createGraphics(width, height, new DefaultFontMapper());
		java.awt.Rectangle area = new java.awt.Rectangle(0, 0, width, height);
		chart.draw(graphics, area);
		graphics.dispose();
		return new ImgTemplate(tp);
	} 

	private Table setupTable(Graph graph) throws BadElementException {
		Table table=new Table(graph.setups().size()+1);
		table.setAutoFillEmptyCells(true);
		table.setSpaceInsideCell(2);
        
		table.setBorderWidth(0);
		table.setDefaultCellBorder(1);
		table.setTableFitsPage(true);
		return table;
	}

	protected void finish(List<TeamCar> cars) {
		try {
			JFreeChart overviewSizeChart = createChart(_overviewSizeDataset,
					ReporterConstants.SIZE_CHART_LEGEND);
			renderOverviewPage(overviewSizeChart,
					ReporterConstants.SIZE_OVERVIEW_LEGEND);

			JFreeChart overviewMemoryChart = createChart(
					_overviewMemoryDataset,
					ReporterConstants.MEMORY_CHART_LEGEND);
			renderOverviewPage(overviewMemoryChart,
					ReporterConstants.MEMORY_OVERVIEW_LEGEND);

			JFreeChart overviewTimeChart = createChart(_overviewTimeDataset,
					ReporterConstants.TIME_CHART_LEGEND);
			renderOverviewPage(overviewTimeChart,
					ReporterConstants.TIME_OVERVIEW_LEGEND);

			renderFirstPage(cars);
			
			renderPDFFile();
		} catch (DocumentException e) {
			e.printStackTrace();
		}
	}
	
	private void renderPDFFile() throws DocumentException {
        Iterator iter = _pdfData.iterator();
        while(iter.hasNext()) {
        	Object obj = iter.next();
        	if(isNewPage(obj)) {
        		_document.newPage();
        	} else {
        		_document.add((Element)obj);
        	}
        }
	}

	private boolean isNewPage(Object obj) {
		return obj instanceof NewPageLabel;
	}

	protected void renderOverviewPage(JFreeChart chart, String legend) throws DocumentException {
		Paragraph para = new Paragraph();
		para.add(new Chunk(legend));
		int i = 0;
		_pdfData.add(i++, para);
		_pdfData.add(i++, renderChart(chart));
		_pdfData.add(i++, new NewPageLabel());
	}
}
