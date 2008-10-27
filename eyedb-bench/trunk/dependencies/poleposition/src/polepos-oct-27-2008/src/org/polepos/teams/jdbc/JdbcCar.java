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

package org.polepos.teams.jdbc;

import java.sql.*;
import java.util.*;

import org.polepos.framework.*;

/**
 * @author Herkules
 */
public class JdbcCar extends Car {

	private static Connection _connection;
	private static Statement _statement;

	private final String _dbType;
	private String _name;
	private boolean _executeBatch;

	private final static Map<Class, String> _colTypesMap = new HashMap<Class, String>();

	static {
		_colTypesMap.put(String.class, "VARCHAR(100)");
		_colTypesMap.put(Integer.TYPE, "INTEGER");
	}

	public JdbcCar(String dbtype) throws CarMotorFailureException {
		_dbType = dbtype;
		JdbcSettings jdbcSettings = Jdbc.settings();
		_website = jdbcSettings.getWebsite(dbtype);
		_description = jdbcSettings.getDescription(dbtype);
		_name = jdbcSettings.getName(dbtype);
		_executeBatch = jdbcSettings.getExecuteBatch(dbtype);

		try {
			Class.forName(jdbcSettings.getDriverClass(dbtype)).newInstance();
		} catch (Exception e) {
			e.printStackTrace();
			throw new CarMotorFailureException();
		}
	}

	public String name() {
		if (_name != null) {
			return _name;
		}
		return _dbType;
	}

	public void openConnection() throws CarMotorFailureException {

		try {
			assert null == _connection : "database has to be closed before opening";
			JdbcSettings jdbcSettings = Jdbc.settings();
			_connection = DriverManager.getConnection(jdbcSettings
					.getConnectUrl(_dbType), jdbcSettings.getUsername(_dbType),
					jdbcSettings.getPassword(_dbType));
			_connection.setAutoCommit(false);
		} catch (SQLException e) {
			e.printStackTrace();
			throw new CarMotorFailureException();
		}
	}

	/**
	 * 
	 */
	public void close() {
		closeStatement();
		commit();
		closeConnection();
	}

	private void closeConnection() {
		try {
			_connection.close();
		} catch (SQLException sqlex) {
			sqlex.printStackTrace();
		}
		_connection = null;
	}

	private void closeStatement() {
		if (_statement != null) {
			try {
				_statement.close();
			} catch (SQLException e) {
				e.printStackTrace();
			} finally {
				_statement = null;
			}
		}
	}

	/**
	 * Commit changes.
	 */
	public void commit() {
		try {
			_connection.commit();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/**
	 * Declarative statement executor
	 */
	public void executeSQL(String sql) {
		try {
			_statement = _connection.createStatement();
			_statement.execute(sql);
		} catch (SQLException ex) {
			ex.printStackTrace();
		} finally {
			closeStatement();
		}
	}

	/**
	 * Declarative statement executor
	 */
	public ResultSet executeQuery(String sql) {
		Log.logger.fine(sql);
		ResultSet rs = null;
		try {
			_statement = _connection.createStatement();
			rs = _statement.executeQuery(sql);
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
		return rs;
	}
	
	public void closeQuery(ResultSet rs) {
		closeResultSet(rs);
		closeStatement();
	}

	private void closeResultSet(ResultSet rs) {
		if(rs != null) {
			try {
				rs.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * Declarative statement executor
	 */
	public ResultSet executeQueryForUpdate(String sql) {
		Log.logger.fine(sql);
		ResultSet rs = null;
		try {
			_statement = _connection.createStatement(ResultSet.TYPE_FORWARD_ONLY,
					ResultSet.CONCUR_UPDATABLE);
			rs = _statement.executeQuery(sql);
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
		return rs;
	}

	public void executeUpdate(String sql) {
		try {
			_statement = _connection.createStatement();
			_statement.executeUpdate(sql);
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			closeStatement();
		}
	}

	/**
	 * Drop a certain table.
	 */
	public void dropTable(String tablename) {
		executeSQL("drop table " + tablename);
	}

	/**
	 * Create a new table, use the first column name as the primary key
	 */
	public void createTable(String tablename, String[] colnames,
			Class[] coltypes) {
		String sql = "create table " + tablename + " (" + colnames[0]
				+ "  INTEGER NOT NULL";

		for (int i = 1; i < colnames.length; i++) {
			sql += ", " + colnames[i] + " " + _colTypesMap.get(coltypes[i]);
		}
		sql += ", PRIMARY KEY(" + colnames[0] + "))";
		executeSQL(sql);
	}

	public void createIndex(String tablename, String colname) {
		// The maximum length for index names is 18 for Derby.
		String sql = "CREATE INDEX X" + tablename + "_" + colname + " ON "
				+ tablename + " (" + colname + ")";
		executeSQL(sql);
	}

	/**
	 * Retrieve a prepared statement.
	 */
	public PreparedStatement prepareStatement(String sql) {
		PreparedStatement stmt = null;
		try {
			stmt = _connection.prepareStatement(sql);
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
		return stmt;
	}

	public boolean executeBatch() {
		return _executeBatch;
	}

}
