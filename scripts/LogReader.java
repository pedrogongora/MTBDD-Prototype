/*  **************************************************************************
 *
 *  Parse log data.
 *
 *  Author: Pedro A. Góngora <pedro.gongora@gmail.com>
 *
 *  Copyright (C) 2012 Pedro A. Góngora
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  *************************************************************************/

import java.io.*;

public class LogReader {

    private BufferedReader in;
    private PrintStream out;
    
    public LogReader(String fname) throws Exception {
        in = new BufferedReader(new FileReader(fname));
        out = System.out;
        
        out.println("n\tk\ta\tEqn\tBF\tSP1\tSP2\tits\tTot\tSize");
        
        String line = line = in.readLine();
        int n,k,a;
        String eqn,bf,sp1,sp2,its,tot,size;
        int remain = 0;
        while ( line != null ) {
            if ( line.startsWith("Reading file:") ) {
                if ( remain > 0 ) {
                    while ( remain > 0 ) {
                        out.print("\t-");
                        remain--;
                    }
                }
                n = atoi(line.substring(line.indexOf("/n")+2, line.indexOf("_k")));
                k = atoi(line.substring(line.indexOf("_k")+2, line.indexOf("_a")));
                a = atoi(line.substring(line.indexOf("_a")+2, line.indexOf(".eqn")));
                out.println();
                out.print(n + "\t" + k + "\t" + a);
                remain = 7;
            } else if ( line.startsWith("--> Building model matrix") ) {
                remain--;
                eqn = line.substring(line.indexOf("took ")+5, line.lastIndexOf("s"));
                out.print("\t" + eqn);
            } else if ( line.startsWith("--> Running Bellman-Ford") ) {
                remain--;
                bf = line.substring(line.indexOf("took ")+5, line.lastIndexOf("s"));
                out.print("\t" + bf);
            } else if ( line.startsWith("--> Computing strategy profiles (step 1/2)") ) {
                remain--;
                sp1 = line.substring(line.indexOf("took ")+5, line.lastIndexOf("s"));
                out.print("\t" + sp1);
            } else if ( line.startsWith("--> Computing strategy profiles (step 2/2)") ) {
                remain--;
                sp2 = line.substring(line.indexOf("took ")+5, line.lastIndexOf("s"));
                out.print("\t" + sp2);
            } else if ( line.startsWith("--> Total time") ) {
                remain--;
                tot = line.substring(line.indexOf("time ")+5, line.lastIndexOf("s"));
                out.print("\t" + tot);
            } else if ( line.startsWith("prev:") ) {
                remain--;
                its = line.substring(line.indexOf("its: ")+5, line.length());
                out.print("\t" + its);
            } else if ( line.startsWith("size:") ) {
                remain--;
                if ( remain > 0 ) {
                    while ( remain > 0 ) {
                        out.print("\t-");
                        remain--;
                    }
                }
                size = line.substring(6, line.lastIndexOf("kb"));
                out.print("\t" + size);
            }
            
            line = in.readLine();
        }
        out.println();
        
        in.close();
    }
    
    private static int atoi(String s) {
        return Integer.parseInt(s);
    }
    
    public static void main(String asrgs[]) throws Exception {
        String fname = "/home/pedrogl/Ubuntu One/workspaceCDT/MTBDD-Proto/Release/log.txt";
        LogReader reader = new LogReader(fname);
        //String line = "Reading file: ../models/n002_k002_a001.eqn";
        //System.out.println(atoi(line.substring(line.indexOf("/n")+2, line.indexOf("_k"))));
        
    }
}
