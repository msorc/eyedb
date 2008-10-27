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

import java.awt.image.*;
import java.io.*;
import java.util.*;

import javax.imageio.*;

import org.apache.velocity.*;
import org.apache.velocity.app.*;
import org.jfree.chart.*;
import org.polepos.framework.*;


public class HTMLReporter extends GraphReporter {	
	public final static String ENCODING = "utf-8";
	
	private File outdir=null;
	private List<Circuit> circuits=new ArrayList<Circuit>();
	private List<Lap> laps=new ArrayList<Lap>();
	private VelocityEngine engine=null;
	private Graph graph=null;
	
	protected void report(Graph graph) {
		try {
			Circuit oldcircuit=circuit();
			if(oldcircuit!=graph.circuit()) {
				if(oldcircuit==null) {
					setup();
				}
				renderCircuitPage();
				circuits.add(graph.circuit());
				laps.clear();
			}	
			this.graph=graph;
			laps.add(graph.lap());
			renderLapGraph(graph);			
			renderLapPage();
		} catch (Exception exc) {
			exc.printStackTrace();
		}	
	}

	private String lapFilePrefix() {
		return circuit().internalName()+"_"+lap().name();
	}

	protected void finish(List <TeamCar> cars) {
		renderOverviewGraph();
		renderOverviewPage();
		renderCircuitPage();
		renderIndexPage();
		copyStylesheet();
	}
	
	public String path() {
	    return super.path() + "/html";
	}
	
	private void setup() throws Exception {
		outdir=new File(path());
		outdir.mkdirs();
		engine = new VelocityEngine();
		engine.setProperty( VelocityEngine.RUNTIME_LOG_LOGSYSTEM, this);
		engine.setProperty(VelocityEngine.ENCODING_DEFAULT,ENCODING);
		engine.setProperty(VelocityEngine.FILE_RESOURCE_LOADER_PATH,getTemplatesDir());
		engine.init();
	}

	private String getTemplatesDir() {
	    String templatesDir = System.getProperty("polepos.templates.dir", "templates");
	    return templatesDir;
	}
	
	private void renderIndexPage() {
        List<TeamCar> distinct = new ArrayList<TeamCar>();
        for(TeamCar teamCar :graph.teamCars()){
            if(!distinct.contains(teamCar)){
                distinct.add(teamCar);
            }
        }
		VelocityContext context = new VelocityContext();
		context.put("includefile", "index.vhtml");
		context.put("teamcars", distinct);
		context.put("circuits", circuits);
		renderPage("index.html", context);
	}

	private void renderCircuitPage() {
		if(circuit()==null) {
			return;
		}
		VelocityContext context = new VelocityContext();
		context.put("includefile","circuitresults.vhtml");
		context.put("circuit",circuit());
		context.put("laps",laps);
		renderPage(circuit().internalName()+".html",context);
	}

	private void renderLapPage() {
		String lapfileprefix=lapFilePrefix();
		VelocityContext context = new VelocityContext();
		context.put("includefile","lapresult.vhtml");
		context.put("graph",graph);
		context.put("fileprefix",lapfileprefix);
		renderPage(lapfileprefix+".html",context);
	}

	private void renderLapGraph(Graph graph) throws IOException {
		JFreeChart timeChart = createTimeChart(graph);
		BufferedImage timeImage = timeChart.createBufferedImage(750, 500);
		ImageIO.write(timeImage, "jpg", new File(outdir, lapFilePrefix()
				+ "_time.jpg"));

		JFreeChart memoryChart = createMemoryChart(graph);
		BufferedImage memoryImage = memoryChart.createBufferedImage(750, 500);
		ImageIO.write(memoryImage, "jpg", new File(outdir, lapFilePrefix()
				+ "_memory.jpg"));
	}
	
	private void renderOverviewGraph() {
		try{
			JFreeChart timeChart = createChart(_overviewTimeDataset, ReporterConstants.TIME_OVERVIEW_LEGEND);
			BufferedImage timeImage = timeChart.createBufferedImage(750, 500);
			ImageIO.write(timeImage, "jpg", new File(outdir, "overview_time.jpg"));
			
			JFreeChart memoryChart = createChart(_overviewMemoryDataset, ReporterConstants.MEMORY_OVERVIEW_LEGEND);
			BufferedImage memoryImage = memoryChart.createBufferedImage(750, 500);
			ImageIO.write(memoryImage, "jpg", new File(outdir, "overview_memory.jpg"));
			
			JFreeChart sizeChart = createChart(_overviewSizeDataset, ReporterConstants.SIZE_OVERVIEW_LEGEND);
			BufferedImage sizeImage = sizeChart.createBufferedImage(750, 500);
			ImageIO.write(sizeImage, "jpg", new File(outdir, "overview_size.jpg"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void renderOverviewPage(){
		VelocityContext context = new VelocityContext();
		context.put("includefile", "overview.vhtml");
		renderPage("overview.html", context);
	}
	
	private void renderPage(String targetName,VelocityContext context) {
		BufferedWriter out=null;
		try {
			Template template=engine.getTemplate("baselayout.vhtml");
			out=new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(outdir,targetName)),ENCODING));
			template.merge(context,out);
			out.close();
		} catch (Exception exc) {
			exc.printStackTrace();
		}
		finally {
			if(out!=null) {
				try {
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	private void copyStylesheet() {
		File sourcefile=new File(new File(getTemplatesDir()),"style.css");
		File targetfile=new File(new File(path()),"style.css");
		copyFile(sourcefile, targetfile);
	}

	private void copyFile(File sourcefile, File targetfile) {
		BufferedInputStream in=null;
		BufferedOutputStream out=null;
		try {
			in=new BufferedInputStream(new FileInputStream(sourcefile));
			out=new BufferedOutputStream(new FileOutputStream(targetfile));
			byte[] buffer=new byte[4096];
			int bytesread=0;
			while((bytesread=in.read(buffer))>=0) {
				out.write(buffer,0,bytesread);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			try {
				if(in!=null) {
					in.close();
				}
				if(out!=null) {
					out.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	private Lap lap() {
		return getLast(laps);
	}

	private Circuit circuit() {
		return getLast(circuits);
	}
	
	private <Item> Item getLast(List<Item> list) {
		if(list.isEmpty()) {
			return null;
		}
		return list.get(list.size()-1);
	}
}
