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
import java.awt.image.*;
import java.io.*;

import javax.imageio.*;

import org.jfree.chart.*;
import org.jfree.chart.axis.*;
import org.jfree.chart.plot.*;
import org.jfree.chart.renderer.category.*;
import org.jfree.data.category.*;
import org.jfree.ui.*;
import org.polepos.framework.*;


public class OverViewChartBuilder {
    
    private static final Font TITLE_FONT = new Font("SansSerif", Font.BOLD, 14);
    private static final Font LEGEND_FONT = new Font("SansSerif", Font.PLAIN, 12);
    private static final Font VALUE_LABEL_FONT = new Font("SansSerif", Font.ITALIC, 12);
    private static final Font VALUE_TICKLABEL_FONT = new Font("SansSerif", Font.PLAIN, 12);
    private static final Font CATEGORY_LABEL_FONT = new Font("SansSerif", Font.ITALIC, 12);
    private static final Font CATEGORY_TICKLABEL_FONT = new Font("SansSerif", Font.PLAIN, 12);
    
    
    private DefaultCategoryDataset _memoryDataset=new DefaultCategoryDataset();
    
    private DefaultCategoryDataset _timeDataset=new DefaultCategoryDataset();
    

    public void report(Graph graph) {
        for (TeamCar teamCar : graph.teamCars()) {
			for (TurnSetup setup : graph.setups()) {
				String legend = graph.circuit().name()
						+ setup.getMostImportantValueForGraph();
				double time = graph.timeFor(teamCar, setup);
				double logTime = Math.log(time + 2);
				double valForOutput = 1 / logTime;
				_timeDataset.addValue(valForOutput, (teamCar.toString()),
						legend);
				long memory = graph.memoryFor(teamCar, setup);
				_memoryDataset.addValue(memory, teamCar.toString(), ":"
						+ legend);
			}
		}
    }
    
    public void createJPGs(String path) {
        File memoryFile = new File(path, "memory.jpg");
        File timeFile = new File(path, "time.jpg");
        createJPG(_memoryDataset, "< better          Memory consumption in kB ", memoryFile);
        createJPG(_timeDataset, " 1  /  log(t + 2)                 better >" , timeFile);
    }
    
    private void createJPG(DefaultCategoryDataset dataset, String legendText, File file){
        
        CategoryAxis categoryAxis = new CategoryAxis("");
        categoryAxis.setLabelFont(CATEGORY_LABEL_FONT);
        categoryAxis.setTickLabelFont(CATEGORY_TICKLABEL_FONT);
        
        ValueAxis valueAxis = new NumberAxis(legendText);
        valueAxis.setLabelFont(VALUE_LABEL_FONT);
        valueAxis.setTickLabelFont(VALUE_TICKLABEL_FONT);
        LineAndShapeRenderer renderer = new LineAndShapeRenderer(true, false);
        CategoryPlot plot = new CategoryPlot(dataset, categoryAxis, valueAxis, renderer);
        plot.setOrientation(PlotOrientation.VERTICAL);
        JFreeChart chart = new JFreeChart("", TITLE_FONT, plot, false);
        StandardLegend legend = new StandardLegend();
        legend.setItemFont(LEGEND_FONT);
        legend.setMargin(new RectangleInsets(1.0, 1.0, 1.0, 1.0));
        legend.setBackgroundPaint(Color.white);
        chart.setLegend(legend);
        
        BufferedImage img=chart.createBufferedImage(1100,900);
        try {
            ImageIO.write(img, "jpg", file);
        } catch (IOException e) {
            e.printStackTrace();
        }
        
    }
    

}
