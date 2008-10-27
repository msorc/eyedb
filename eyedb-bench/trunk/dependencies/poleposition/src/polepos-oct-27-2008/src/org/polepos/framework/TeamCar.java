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

package org.polepos.framework;


public class TeamCar {
    
    private final Team team;
    private final Car car;
    
    public TeamCar(Team team, Car car){
        this.team = team;
        this.car = car;
    }
    
    @Override
    public boolean equals(Object obj) {
        if(obj==this) {
            return true;
        }
        if(obj==null||obj.getClass()!=getClass()) {
            return false;
        }
        TeamCar key=(TeamCar)obj;
        return team.equals(key.team) && car.equals(key.car);
    }
    
    @Override
    public int hashCode() {
        return team.hashCode() + car.hashCode();
    }
    
    public String toString(){
        return team.name() + "/" + car.name();
    }
    
    public String name(){
        return team.name() + "/" + car.name();
    }
    
    public String description(){
        if(team.website() != null){
            return team.description();
        }
        return car.description();
    }
    
    public Team getTeam(){
        return team;
    }
    
    public Car getCar(){
        return car;
    }
    
    public String website(){
        String webSite = team.website();
        if(webSite != null){
            return webSite;
        }
        return car.website();
    }

    
}
