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

public class ReporterConstants {
	protected final static String WEBSITE="http://www.polepos.org";
    public final static int TIME = 1;
    public final static int MEMORY = 2;
	public static final String TIME_CHART_LEGEND = " 1  /  log(t + 2)                     better >";
    public static final String MEMORY_CHART_LEGEND = " 1  /  log(m + 2)                     better >";
    public static final String SIZE_CHART_LEGEND = " 1  /  log(b + 2)                     better >";
    public static final String TIME_OVERVIEW_LEGEND = "Time Overview\n\n";
    public static final String MEMORY_OVERVIEW_LEGEND = "Memory Overview\n\n";
    public static final String SIZE_OVERVIEW_LEGEND = "Database Size Overview\n\n";
    public static final Font TITLE_FONT = new Font("SansSerif", Font.BOLD, 14);
    public static final Font LEGEND_FONT = new Font("SansSerif", Font.PLAIN, 12);
    public static final Font VALUE_LABEL_FONT = new Font("SansSerif", Font.ITALIC, 12);
    public static final Font VALUE_TICKLABEL_FONT = new Font("SansSerif", Font.PLAIN, 10);
	public static final Font CATEGORY_LABEL_FONT = new Font("SansSerif", Font.ITALIC, 12);
	public static final Font CATEGORY_TICKLABEL_FONT = new Font("SansSerif", Font.PLAIN, 10);

}
